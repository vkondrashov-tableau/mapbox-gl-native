#pragma once
#include <mbgl/renderer/bucket_parameters.hpp>
#include <mbgl/geometry/feature_index.hpp>

namespace mbgl {

template <class B, class L>
class PatternLayout {
public:
    PatternLayout(const BucketParameters& parameters,
                  const std::vector<const RenderLayer*>& layers,
                  std::unique_ptr<GeometryTileLayer> sourceLayer_,
                  ImageDependencies& patternDependencies)
                  : bucketLeaderID(layers.at(0)->getID()),
                    sourceLayer(std::move(sourceLayer_)),
                    zoom(parameters.tileID.overscaledZ),
                    overscaling(parameters.tileID.overscaleFactor()) {

        const L* renderLayer = layers.at(0)->as<L>();
        const typename L::StyleLayerImpl& leader = renderLayer->impl();
        layout = leader.layout.evaluate(PropertyEvaluationParameters(zoom));
        sourceLayerID = leader.sourceLayer;
        groupID = renderLayer->getID();

        for (const auto& layer : layers) {
            const typename B::PossiblyEvaluatedPaintProperties evaluatedProps = layer->as<L>()->paintProperties();
            layerPaintProperties.emplace(layer->getID(), std::move(evaluatedProps));
            const auto patterns = evaluatedProps.template get<typename L::PatternProperty>().possibleOutputs();

            for (auto& pattern : patterns) {
                const auto patternString = pattern.value_or("");
                if (!patternString.empty()) {
                    patternDependencies.emplace(patternString, ImageType::Pattern);
                }
            }
        }
        const size_t featureCount = sourceLayer->featureCount();
        for (size_t i = 0; i < featureCount; ++i) {
            auto feature = sourceLayer->getFeature(i);
            if (!leader.filter(style::expression::EvaluationContext { this->zoom, feature.get() }))
                continue;
            features.push_back({i, std::move(feature)});
        }
    };

    std::unique_ptr<B> createBucket(const ImagePositions& patternPositions, std::unique_ptr<FeatureIndex>& featureIndex) {
        auto bucket = std::make_unique<B>(layout, layerPaintProperties, zoom, overscaling);
        for (auto & pair : features) {
            std::unique_ptr<GeometryTileFeature> feature = std::move(pair.second);
            const auto i = pair.first;
            GeometryCollection geometries = feature->getGeometries();

            bucket->addFeature(*feature, geometries, patternPositions);
            featureIndex->insert(geometries, i, sourceLayerID, groupID);
        }
        return bucket;
    };

    std::map<std::string, typename B::PossiblyEvaluatedPaintProperties> layerPaintProperties;

    const std::string bucketLeaderID;
private:
    const std::unique_ptr<GeometryTileLayer> sourceLayer;
    std::vector<std::pair<uint32_t, std::unique_ptr<GeometryTileFeature>>> features;
    typename B::PossiblyEvaluatedLayoutProperties layout;

    const float zoom;
    const uint32_t overscaling;
    std::string sourceLayerID;
    std::string groupID;
};

} // namespace mbgl

