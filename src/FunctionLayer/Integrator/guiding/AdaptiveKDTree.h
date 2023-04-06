#pragma once

#include <vector>

#ifdef _WIN32
#include <ppl.h>
#else
#include <omp.h>
#endif

#include "GuidedBxDF.h"

namespace PathGuiding::kdtree {

using SampleIterator = std::vector<SampleData>::iterator;
using ParallaxAwareVMM = vmm::ParallaxAwareVMM;

class Region {
public:

    Vec3d posMean;
    Vec3d posVar;
    ParallaxAwareVMM * model{};
    int numSamples{};

    inline explicit Region(ParallaxAwareVMM * model) {
        this->model = model;
    }

    virtual ~Region() {
        delete model;
    }

    inline void updateStats(SampleIterator begin, SampleIterator end) {
        auto iter = begin;
        if (numSamples == 0) {
            posMean = iter->position;
            posVar = Vec3d(0);
            numSamples += 1;
            ++iter;
        }
        while (iter != end) {
            Vec3d diff = iter->position - posMean;
            posMean += diff / (numSamples + 1.);
            posVar += diff * diff / (numSamples + 1.) - posVar / numSamples;
            numSamples += 1;
            ++iter;
        }
    }

    inline void clearStats() {
        numSamples = 0;
        posMean = Vec3d(0);
        posVar = Vec3d(0);
    }
};

class AdaptiveKDTree {
private:

    constexpr static int maxDepth = 17;

    struct Node {
        union {
            struct { Node * children[2]; };
            struct { Node * parent; Region * region; };
        };
        double splitPos;
        int splitAxis;

        // Create a leaf node
        inline Node(Region * region, Node * parent) {
            this->region = region;
            this->parent = parent;
            this->splitAxis = -1;
            this->splitPos = 0;
        }

        virtual ~Node() {
            if (splitAxis < 0) {
                delete region;
            } else {
                delete children[0];
                delete children[1];
            }
        }
    };

    Node * root{};

public:

    inline AdaptiveKDTree() {
        auto model = new ParallaxAwareVMM();
        auto region = new Region(model);
        root = new Node(region, nullptr);
    }

    virtual ~AdaptiveKDTree() {
        delete root;
    }

    // configure the BxDF to be prepared for sampling and pdf evaluation at the given position
    inline void getGuidedBxDF(GuidedBxDF & guidedBxDF, BxDF * bxdf, const Vec3d & position) const {
        Node * currentNode = root;
        while (currentNode->splitAxis >= 0) {
            int childIdx = (position[currentNode->splitAxis] < currentNode->splitPos ? 0 : 1);
            currentNode = currentNode->children[childIdx];
        }

        auto region = currentNode->region;
        region->model->getWarped(guidedBxDF.model, position);
        guidedBxDF.bxdf = bxdf;
    }

    inline void update(std::vector<SampleData> & samples, int nThreads) {
#ifndef _WIN32
#pragma omp parallel num_threads(nThreads), default(none), shared(samples)
#pragma omp single nowait
#endif
        updateNode(root, samples.begin(), samples.end(), 0);
    }

private:

    inline static int argmax(const Vec3d & v) {
        return v[0] >= v[1] ? (v[0] >= v[2] ? 0 : 2) : (v[1] >= v[2] ? 1 : 2);
    }

    static void updateNode(Node * node, SampleIterator begin, SampleIterator end, int depth) {
        if (begin >= end) {
            return;
        }

        const int maxRegionNumSamples = 32768;

        if (node->splitAxis < 0) {
            auto region = node->region;
            region->updateStats(begin, end);
            if (region->numSamples < maxRegionNumSamples) {
                region->model->update(region->posMean, begin, end);
                return;
            } else if (depth < maxDepth) {
                node->splitAxis = argmax(region->posVar);
                node->splitPos = region->posMean[node->splitAxis];
                region->clearStats();

                auto leftChild = new Node(region, node);
                auto rightRegion = new Region(region->model->split());
                auto rightChild = new Node(rightRegion, node);

                node->children[0] = leftChild;
                node->children[1] = rightChild;
            } else {
                std::cout << "[WARN] AdaptiveKDTree: reach maxDepth" << std::endl;
            }
        }

        if (node->splitAxis >= 0) {
            auto middle = std::partition(begin, end, [&](const SampleData & sample) -> bool {
                return sample.position[node->splitAxis] < node->splitPos;
            });

#ifdef _WIN32
            if (std::distance(begin, end) < 512) {
                updateNode(node->children[0], begin, middle, depth + 1);
                updateNode(node->children[1], middle, end, depth + 1);
            } else {
                Concurrency::parallel_invoke(
                    [&] { updateNode(node->children[0], begin, middle, depth + 1); },
                    [&] { updateNode(node->children[1], middle, end, depth + 1); });
            }
#else
#pragma omp task mergeable default(none), firstprivate(node, begin, middle, depth)
            updateNode(node->children[0], begin, middle, depth + 1);
#pragma omp task mergeable default(none), firstprivate(node, middle, end, depth)
            updateNode(node->children[1], middle, end, depth + 1);
#endif
        }
    }
};

}
