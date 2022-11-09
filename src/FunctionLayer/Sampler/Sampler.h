/**
 * @file Sampler.h
 * @author orbitchen
 * @brief generate random numbers between (0,1).
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <vector>
#include "CoreLayer/Adapter/Random.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Math/Common.h"

struct CameraSample;

/// \defgroup Sampler
/**
 * @brief Base class for all samplers
 * @ingroup Sampler
   @todo Handle the multi-threads issue
 */
class Sampler
{
protected:
	RandomNumberGenerator rng;
public:
	Sampler() = default;

	virtual ~Sampler() = default;

	virtual void startPixel(const Point2i &pixelPosition) = 0;

	virtual void nextSample() = 0;
	
	virtual double sample1D() = 0;

	virtual Point2d sample2D() = 0;

  /**
   * @brief Return a copy of this Sampler instance,
   *        in order to secure the multi-thread sampling.
   * @note  Copy of a Sampler is to persist the sampling strategy,
   *        not identical sampling sequences.
   * 
   * @param seed Seed of rng, to make random sequence UNIQUE for each copy.
   *             May not be used, because RandomNumberGenerator uses random seeds.
   * @return std::unique_ptr<Sampler> Copy of this Sampler
   * 
   * @date 2022-10-15
   * @author ja50n
   */
  virtual std::unique_ptr<Sampler> clone(int seed) const = 0;

	CameraSample getCameraSample();

};

/**
 * @brief Shuffle the vector samples
 * 
 * @tparam T 
 * @param samples 
 */
template<typename T>
void shuffle(std::vector<T> &samples, RandomNumberGenerator &rng) {
	int count = samples.size();
	for (int i = 0; i < count; ++i) {
		int other = rng(i, count);
		std::swap(samples[i], samples[other]);
	}
}

/**
 * @brief Base class for pixel sampler, which generate specific samples before rendering each pixel
 *  need the dimensions and samplesPerPixels when construct
 * @ingroup Sampler
 */
class PixelSampler : public Sampler {
protected:
	Point2i pixelPosition;
	std::vector<std::vector<double>>  samples1D;
	std::vector<std::vector<Point2d>> samples2D;
	int sppSqrt;
	int samplesPerPixel;
	int nDimensions;
	int curSamplePixelIndex;
	int curDimensionIndex1D;
	int curDimensionIndex2D;

	virtual void generateSamples1D(std::vector<double> &samples) = 0;
	virtual void generateSamples2D(std::vector<Point2d> &samples) = 0; 

public:
	PixelSampler() = delete;
	
	PixelSampler(int _sppSqrt)
		: sppSqrt(_sppSqrt), 
		  samplesPerPixel(_sppSqrt * _sppSqrt), 
		  nDimensions(5), 
		  curSamplePixelIndex(0), 
		  curDimensionIndex1D(0), 
		  curDimensionIndex2D(0) 
	{
		for (int i = 0; i < nDimensions; ++i) {
			samples1D.emplace_back(std::vector<double>(samplesPerPixel));
			samples2D.emplace_back(std::vector<Point2d>(samplesPerPixel));
		}
	}
	
	PixelSampler(int _sppSqrt, int _nDimensions)
		: sppSqrt(_sppSqrt), 
		  samplesPerPixel(_sppSqrt * _sppSqrt), 
		  nDimensions(_nDimensions), 
		  curSamplePixelIndex(0), 
		  curDimensionIndex1D(0), 
		  curDimensionIndex2D(0) 
	{ 
		for (int i = 0; i < nDimensions; ++i) {
			samples1D.emplace_back(std::vector<double>(samplesPerPixel));
			samples2D.emplace_back(std::vector<Point2d>(samplesPerPixel));
		}
	}
	
	virtual ~PixelSampler() = default;
	
	/// @brief The sampler may change the sampling stragety at different pixel location, record the current pixel location
	/// @param _pixelPositon location of current pixel
	virtual void startPixel(const Point2i &_pixelPositon) override {
		// record the position
		pixelPosition = _pixelPositon;
		// reset the sample index
		curDimensionIndex1D = curDimensionIndex2D = curSamplePixelIndex = 0;
		// generate the samples will be used in current pixel
		for (int i = 0; i < nDimensions; ++i) {
			generateSamples1D(samples1D[i]);
			shuffle(samples1D[i], rng);
			generateSamples2D(samples2D[i]);
			shuffle(samples2D[i], rng);
		}
	}

	/// @brief Start the next sample at a specific pixel
	virtual void nextSample() override {
		++curSamplePixelIndex;
		curDimensionIndex1D = curDimensionIndex2D = 0;
	}

	/// @brief Sample a double in [0, 1], if curDimension exceeds the nDimensions, just return rng()
	/// @return A double sample
	virtual double sample1D() override {
		if (curDimensionIndex1D < nDimensions)
			return samples1D[curDimensionIndex1D++][curSamplePixelIndex];
		else
			return rng();
	}

	/// @brief Sample a point2d in [0, 1]^2, if curDimension exceeds the nDimensions, just return {rng(), rng()}
	/// @return A point2d sample	
	virtual Point2d sample2D() override {
		if (curDimensionIndex2D < nDimensions)
			return samples2D[curDimensionIndex2D++][curSamplePixelIndex];
		else
		 	return Point2d{rng(), rng()};
	}
};

/**
 * @brief Some sampling strategies have better performance when generating
 *        samples over all the image. (low-discrepancy sequences, etc.)
 *        GlobalSampler is to warp them in order to fit the
 *        pixel-by-pixel interface.
 *        It maintains samples for all pixels and picks sample
 *        values through some mapping.
 * @note  By now the GlobalSampler generates 1 or 2 values each time through
 *        calling sample1D() or sample2D().
 * @author ja50n
 */
class GlobalSampler : public Sampler {
 public:
  GlobalSampler() = delete;
  GlobalSampler(int64_t _sppSqrt)
      : GlobalSampler(_sppSqrt, 5) {
  }
  GlobalSampler(int64_t _sppSqrt, int _nDimensions)
      : sppSqrt(_sppSqrt),
        samplesPerPixel(_sppSqrt * _sppSqrt),
        nDimensions(_nDimensions),
        nextDimension(0),
        curSampleGlobalIndex(0),
        curSamplePixelIndex(0),
        curDimensionIndex1D(0),
        curDimensionIndex2D(0) {
    for (int i = 0; i < nDimensions; i++) {
      samples1D.emplace_back(std::vector<double>(samplesPerPixel));
      samples2D.emplace_back(std::vector<Point2d>(samplesPerPixel));
    }
  }
  virtual ~GlobalSampler() = default;

  virtual void startPixel(const Point2i &_pixelPosition) override {
    pixelPosition = _pixelPosition;
    // Reset dimension
    nextDimension = 0;
    // Reset sample index
    curDimensionIndex1D = curDimensionIndex2D = curSamplePixelIndex = 0;
    // Get index of the first sample in global sequence
    curSampleGlobalIndex = globalSampleIndex(0);
    // Generate 1D array
    for (int i = 0; i < nDimensions; i++) {
      for (int64_t j = 0; j < samplesPerPixel; j++) {
        // Fill the pixel sample array with some global sequence
        int64_t idx = globalSampleIndex(j);
        samples1D[i][j] = sampleValue(idx, i);
      }
    }
    // Generate 2D array
    // To avoid the identical values
    // in case that low-discrepancy sequence is deterministic
    int dim = nDimensions;
    for (size_t i = 0; i < nDimensions; i++) {
      for (int64_t j = 0; j < samplesPerPixel; j++) {
        int64_t idx = globalSampleIndex(j);
        // In fact it goes further than nDimensions
        samples2D[i][j].x = sampleValue(idx, dim);
        samples2D[i][j].y = sampleValue(idx, dim + 1);
      }
      dim += 2;
    }
  }

  virtual void nextSample() override {
    // Reset the dimension
    nextDimension = 0;
    // Increase the global index
    curSampleGlobalIndex = globalSampleIndex(curSamplePixelIndex + 1);
    curSamplePixelIndex++;
    // Reset the in-sample index
    curDimensionIndex1D = curDimensionIndex2D = 0;
    /// @note This assertion is a compromise with
    ///       nextSample() not indicating end of vector
    assert(curSamplePixelIndex < samplesPerPixel);
  }

  virtual double sample1D() override {
    if (curDimensionIndex1D < nDimensions) 
      return samples1D[curDimensionIndex1D++][curSamplePixelIndex];
    else
      return rng();
  }

  virtual Point2d sample2D() override {
    if (curDimensionIndex2D < nDimensions) 
      return samples2D[curDimensionIndex2D++][curSamplePixelIndex];
    else
      return Point2d{rng(), rng()};
  }

  /**
   * @brief Map current pixel position and current pixel sample index
   *        to the global sample index.
   *
   * @param pixelSampleIndex Index of current pixel sample.
   * @return int64_t Index in global samples.
   */
  virtual int64_t globalSampleIndex(int64_t pixelSampleIndex) const = 0;

  /**
   * @brief Get the specified sample value from the global sequence.
   *
   * @param globalIndex Index of global sample sequence.
   * @param dim Index of dimension, start with 0.
   * @return double PIXEL RELATED sample value, should be in [0, 1).
   */
  virtual double sampleValue(int64_t globalIndex, int dim) const = 0;

  const int64_t samplesPerPixel;
  const int64_t sppSqrt;

 protected:
  // Position of current sampling pixel, set by startPixel().
  Point2i pixelPosition;
  // Samples
  // Layout: samples[i][j] is the i-th (dimension) value of j-th sample.
  std::vector<std::vector<double>> samples1D;
  std::vector<std::vector<Point2d>> samples2D;
  // Number of dimensions per sample
  int nDimensions;
  // Index of current sample in one pixel
  int64_t curSamplePixelIndex;
  // Index of sample dimension
  size_t curDimensionIndex1D;
  size_t curDimensionIndex2D;

 private:
  // The next dimension, incremented when sample1D() or sample2D() is
  // called
  size_t nextDimension;
  // Index of current sample in the global sequence
  int64_t curSampleGlobalIndex;
};