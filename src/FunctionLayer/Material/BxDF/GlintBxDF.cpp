#include "GlintBxDF.h"
#include "Fresnel.h"
#include "CoreLayer/Geometry/Frame.h"

GlintBxDF::GlintBxDF(Vec3d _eta, Vec3d _k, Spectrum _albedo, double _uRoughness, double _vRoughness,Intersection _it,
                                       std::shared_ptr < MicrofacetDistribution > _distrib) :
                                        eta(_eta),k(_k),albedo(_albedo),it(_it),distrib(std::move(_distrib))
                                       {
    alphaXY = Vec2d(distrib->roughnessToAlpha(_uRoughness),distrib->roughnessToAlpha(_vRoughness));
                                       }

BxDFSampleResult GlintBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result{};
    if(CosTheta(out)<0){
        return result;
    }
    Vec3d wh = distrib->Sample_wh(out,sample,alphaXY);
    Vec3d in = Frame::reflect(out,wh);
    if (dot(wh,out) < 0 || CosTheta(in)<0)
    {
        result.s=Spectrum(0);
        return result;
    }
    double mPdf =distrib->Pdf(out,wh,alphaXY);
    auto woDoth = dot(out,wh);
    double pdf = mPdf*0.25f/woDoth;
    Spectrum F = Fresnel::conductorReflectance(eta, k, woDoth);
    result.pdf = pdf;
    result.s = ( F * distrib->G(out,in,alphaXY)*distrib->D(wh,alphaXY))/abs(4 * out.z * in.z);
    result.directionIn = in;
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    return result;
}

double GlintBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    if ( CosTheta(out) < 0 || CosTheta(in) < 0) return 0;
    Vec3d wh = normalize(out + in);
    return distrib->Pdf(out, wh,alphaXY) / (4 * dot(out, wh));
}

// count particles number fall in tex area
double GlintBxDF::count_spatial(BoundingBox2d &queryBox) const {
    uint32_t countNum = 0, queryDepth = 0;
    std::vector<SpatialNode> queue;
    queue.push_back(std::make_pair(BoundingBox2d(Point2d(0.f, 0.f), Point2d(1.f, 1.f)), SPATIAL_SAMPLE_NUM));
    SpatialNode node;
    BoundingBox2d clipBox;
    double p, w;
    //bfs
    while (!queue.empty()){
        node = queue.back();
        queue.pop_back();
        if(!queryBox.overlap(node.first) || node.second <= 0){
            continue;
        }
        else if(queryBox.contains(node.first)){
            countNum += node.second;
        }
        else{
            p = countNum / SPATIAL_SAMPLE_NUM;
            // approximation
            if(sqrt(node.second * p * (1 - p)) < EPSILON * countNum){
                clipBox = node.first;
                clipBox = BoundingBoxUnionIntersect(queryBox, clipBox);
                countNum += (uint32_t)(node.second * clipBox.getSurfaceArea() / node.first.getSurfaceArea());
            }
            else{
                if(queryDepth >= MAX_QUERY_DEPTH){
                    break;
                }
                // split box
                w = 1 - queryPs[queryDepth][0] - queryPs[queryDepth][1] - queryPs[queryDepth][2];
                Point2d center = node.first.getCenter(), pMin = node.first.pMin, pMax = node.first.pMax;
                queue.push_back(std::make_pair(
                    BoundingBox2(pMin, center), 
                    (uint32_t)(node.second * queryPs[queryDepth][0])));
                queue.push_back(std::make_pair(
                    BoundingBox2(Point2d(pMin[0], center[1]), Point2d(center[0],pMax[1])), 
                    (uint32_t)(node.second * queryPs[queryDepth][1])));
                queue.push_back(std::make_pair(
                    BoundingBox2(Point2d(center[0], pMin[1]), Point2d(pMax[0],center[1])), 
                    (uint32_t)(node.second * queryPs[queryDepth][2])));
                queue.push_back(std::make_pair(
                    BoundingBox2(center, pMax), 
                    (uint32_t)(node.second * w)));
                queryDepth++;
            }
        }
    }
    assert(countNum < SPATIAL_SAMPLE_NUM);
    return (double)countNum / SPATIAL_SAMPLE_NUM;
    

}

// count particles number fall in cone
double GlintBxDF::count_direction(const Vec3d &wi, const Vec3d & wo) const{
    uint32_t countNum = 0, queryDepth = 1;
    std::vector<DirectionNode> queue;
    ConicQuery query(wi, wo);
    queue.push_back(std::make_pair(DirTriangle(Point2d(0.f, 0.f), Point2d(M_PI / 2, 0.f), Point2d(0.f, M_PI /2)),
                                    (uint32_t)(DIRECTION_SAMPLE_NUM * queryPs[0].x)));
    queue.push_back(std::make_pair(DirTriangle(Point2d(M_PI / 2, 0.f), Point2d(M_PI, 0.f), Point2d(0.f, M_PI /2)),
                                    (uint32_t)(DIRECTION_SAMPLE_NUM * queryPs[0].y)));
    queue.push_back(std::make_pair(DirTriangle(Point2d(M_PI , 0.f), Point2d(M_PI * 3 / 2, 0.f), Point2d(0.f, M_PI /2)),
                                    (uint32_t)(DIRECTION_SAMPLE_NUM * queryPs[0].z)));
    queue.push_back(std::make_pair(DirTriangle(Point2d(M_PI * 3 / 2, 0.f), Point2d(M_PI * 2, 0.f), Point2d(0.f, M_PI /2)),
                                    (uint32_t)(DIRECTION_SAMPLE_NUM * (1 - queryPs[0].x - queryPs[0].y - queryPs[0].z))));
    DirectionNode node;
    double p;
    //bfs
    while (!queue.empty()){
        node = queue.back();
        queue.pop_back();
        // not overlap
        if((!(query.inConic(node.first[0]) || query.inConic(node.first[1]) || 
                                query.inConic(node.first[2]))) || node.second <= 0){
            continue;
        }
        // contains
        else if(query.conicContain(node.first)){
            countNum += node.second;
        }
        else{
            p = countNum / SPATIAL_SAMPLE_NUM;
            // approximation
            // todo: calculate the ratio of area
            if(sqrt(node.second * p * (1 - p)) < EPSILON * countNum){
                countNum += node.second;
            }
            else{
                if(queryDepth >= MAX_QUERY_DEPTH){   
                    break;
                }
                // split triangle
                queue.push_back(std::make_pair(DirTriangle(node.first.edgeMidPoint(0), node.first.edgeMidPoint(1), node.first.edgeMidPoint(2)),
                                                (uint32_t)(node.second * queryPs[queryDepth].x)));
                queue.push_back(std::make_pair(DirTriangle(node.first.edgeMidPoint(0), node.first.edgeMidPoint(2), node.first[0]),
                                                (uint32_t)(node.second * queryPs[queryDepth].y)));      
                queue.push_back(std::make_pair(DirTriangle(node.first.edgeMidPoint(0), node.first.edgeMidPoint(1), node.first[1]),
                                                (uint32_t)(node.second * queryPs[queryDepth].z))); 
                queue.push_back(std::make_pair(DirTriangle(node.first.edgeMidPoint(1), node.first.edgeMidPoint(2), node.first[2]),
                                                (uint32_t)(node.second * (1- queryPs[0].x - queryPs[0].y - queryPs[0].z))));     
                queryDepth++;                                                           
            }
        }
    }
    return (double)countNum / DIRECTION_SAMPLE_NUM;
}

Spectrum GlintBxDF::f(const Vec3d & out, const Vec3d & in) const {
   if(out.z<0 || in.z<0)
        return {0.};
    double cosThetaO = AbsCosTheta(out), cosThetaI = AbsCosTheta(in);
    Vec3d wh = in + out;
    // Handle degenerate cases for microfacet reflection
    if (cosThetaI == 0 || cosThetaO == 0) return {0.};
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return {0.};
    wh = normalize(wh);
    double cosI = dot(out,wh.z>0?wh:-wh);
    auto F = Fresnel::conductorReflectance(eta,k,cosI);
    auto G = distrib->G(out, in,alphaXY);

    double du = (it.dudx + it.dudy) / 2, dv = (it.dvdx + it.dvdy) / 2;
    BoundingBox2d texBox;
    texBox = BoundingBoxPointUnion(texBox, Point2d(du, dv));
    texBox = BoundingBoxPointUnion(texBox, Point2d(-du, dv));
    texBox = BoundingBoxPointUnion(texBox, Point2d(du, -dv));
    texBox = BoundingBoxPointUnion(texBox, Point2d(-du, -dv));

    double D = count_direction(in, out);
    D *= count_spatial(texBox);
    if(D < 1e-8)
        return {0.};
    return albedo * (dot(in,wh) * F * D * G) / 
            (texBox.getSurfaceArea() * M_PI * (1 - cos(GAMMA)) * cosThetaI * cosThetaO);
}