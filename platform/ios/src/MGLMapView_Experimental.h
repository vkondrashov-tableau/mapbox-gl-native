#import <Mapbox/Mapbox.h>

@interface MGLMapView (Experimental)

/// Rendering performance measurement.
@property (nonatomic) BOOL experimental_enableFrameRateMeasurement;
@property (nonatomic, readonly) CGFloat averageFrameRate;
@property (nonatomic, readonly) CFTimeInterval frameTime;
@property (nonatomic, readonly) CFTimeInterval averageFrameTime;

@end
