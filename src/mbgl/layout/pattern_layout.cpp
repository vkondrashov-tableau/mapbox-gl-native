#include <mbgl/layout/pattern_layout.hpp>


namespace mbgl {

using namespace style;

PatternLayout::PatternLayout(const BucketParameters& parameters,
                  const std::vector<const RenderLayer*>& layers,
                  std::unique_ptr<GeometryTileLayer> sourceLayer_,
                  ImageDependencies& patternDependencies)
                  : bucketLeaderID(layers.at(0)->getID()),
                    sourceLayer(std::move(sourceLayer_)),
                    zoom(parameters.tileID.overscaledZ),
                    overscaling(parameters.tileID.overscaleFactor()) {

    const RenderLineLayer* renderLayer = layers.at(0)->as<RenderLineLayer>();
    const LineLayer::Impl& leader = renderLayer->impl();
    layout = leader.layout.evaluate(PropertyEvaluationParameters(zoom));
    sourceLayerID = leader.sourceLayer;
    groupID = renderLayer->getID();

    for (const auto& layer : layers) {
        const RenderLinePaintProperties::PossiblyEvaluated evaluatedProps = layer->as<RenderLineLayer>()->paintProperties();
        layerPaintProperties.emplace(layer->getID(), std::move(evaluatedProps));
        const auto patterns = evaluatedProps.get<LinePattern>().possibleOutputs();

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
        if (!leader.filter(expression::EvaluationContext { this->zoom, feature.get() }))
            continue;
        features.push_back({i, std::move(feature)});
    }
}

std::unique_ptr<LineBucket> PatternLayout::createBucket(const ImagePositions& patternPositions, std::unique_ptr<FeatureIndex>& featureIndex) {
    auto bucket = std::make_unique<LineBucket>(layout, layerPaintProperties, zoom, overscaling);
    for (auto & pair : features) {
        std::unique_ptr<GeometryTileFeature> feature = std::move(pair.second);
        const auto i = pair.first;
        GeometryCollection geometries = feature->getGeometries();

        bucket->addFeature(*feature, geometries, patternPositions);
        featureIndex->insert(geometries, i, sourceLayerID, groupID);
    }
    return bucket;
};

} // namespace mbgl
