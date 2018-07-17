package com.mapbox.mapboxsdk.maps;

import com.mapbox.android.telemetry.SessionInterval;

/**
 * @deprecated use {@link com.mapbox.mapboxsdk.module.telemetry.Telemetry} instead.
 */
public class Telemetry {

  /**
   * Set the debug logging state of telemetry.
   *
   * @param debugLoggingEnabled true to enable logging
   * @deprecated use {@link com.mapbox.mapboxsdk.module.telemetry.Telemetry#updateDebugLoggingEnabled(boolean)} instead
   */
  @Deprecated
  public static void updateDebugLoggingEnabled(boolean debugLoggingEnabled) {
    com.mapbox.mapboxsdk.module.telemetry.Telemetry.updateDebugLoggingEnabled(debugLoggingEnabled);
  }

  /**
   * Update the telemetry rotation session id interval
   *
   * @param interval the selected session interval
   * @return true if rotation session id was updated
   * @deprecated use {@link com.mapbox.mapboxsdk.module.telemetry.Telemetry#setSessionIdRotationInterval(int)} instead
   */
  @Deprecated
  public static boolean updateSessionIdRotationInterval(SessionInterval interval) {
    return com.mapbox.mapboxsdk.module.telemetry.Telemetry.updateSessionIdRotationInterval(interval);
  }

  /**
   * Method to be called when an end-user has selected to participate in telemetry collection.
   *
   * @deprecated use {@link com.mapbox.mapboxsdk.module.telemetry.Telemetry#setUserTelemetryRequestState(boolean)}
   * with parameter true instead
   */
  @Deprecated
  public static void enableOnUserRequest() {
    com.mapbox.mapboxsdk.module.telemetry.Telemetry.enableOnUserRequest();
  }

  /**
   * Method to be called when an end-user has selected to opt-out of telemetry collection.
   *
   * @deprecated use {@link com.mapbox.mapboxsdk.module.telemetry.Telemetry#setUserTelemetryRequestState(boolean)}
   * with parameter false instead
   */
  @Deprecated
  public static void disableOnUserRequest() {
    com.mapbox.mapboxsdk.module.telemetry.Telemetry.disableOnUserRequest();
  }
}