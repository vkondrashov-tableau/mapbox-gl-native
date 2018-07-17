package com.mapbox.mapboxsdk.module;

import com.mapbox.mapboxsdk.http.HttpRequest;
import com.mapbox.mapboxsdk.maps.TelemetryBase;
import com.mapbox.mapboxsdk.module.http.HttpRequestImpl;
import com.mapbox.mapboxsdk.module.telemetry.Telemetry;

/**
 * Injects concrete instances of configurable abstractions
 */
public class ModuleProvider {

  /**
   * Create a new concrete implementation of HttpRequest.
   *
   * @return a new instance of an HttpRequest
   */
  public static HttpRequest injectHttpRequest() {
    return new HttpRequestImpl();
  }

  /**
   * Get the concrete implementation of TelemetryBase
   *
   * @return a single instance of Telemetry
   */
  public static TelemetryBase injectTelemetry() {
    // TODO remove singleton with next major release,
    // this is needed to make static methods on Telemetry
    // backwards compatible without breaking semver
    return Telemetry.getInstance();
  }
}
