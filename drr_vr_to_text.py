import cv2
import pandas as pd
import math

# metadata from deployment -- update this for each unique deployment
# incline periphyton
lat_o = 39.24719 # N
long_o = -119.99553 # W
heading_deg = 60 # degrees clockwise from north
radius = 6379.8 # km

# function for calculating the offset from origin coords using drr's pos x, y, and z
def offset_to_latlon(x_fwd, y_right, lat_origin, long_origin, radius_km, heading_deg):
    """
    Convert a body-frame offset (meters), given in the AUV's own
    forward/right axes, into an updated (lat, long).

    heading_deg: compass heading (where 0=N, 90=E, 180=S, 270=W) that the AUV
                 was facing at the moment it was zeroed at the origin.
    """
    radius_m = radius_km * 1000
    heading_rad = math.radians(heading_deg)

    # rotate body-frame (forward, right) into geographic (east, north)
    east = x_fwd * math.sin(heading_rad) + y_right * math.cos(heading_rad)
    north = x_fwd * math.cos(heading_rad) - y_right * math.sin(heading_rad)

    delta_lat_rad = north / radius_m
    delta_lon_rad = east / (radius_m * math.cos(math.radians(lat_origin)))

    new_lat = lat_origin + math.degrees(delta_lat_rad)
    new_long = long_origin + math.degrees(delta_lon_rad)

    return new_lat, new_long

def main():

    videoPath = r'D:\output_20260516_190623_front.mp4'
    drrPath = r'C:\Users\Meissa Vaccines\Downloads\Cyclone_VideoFeed\2026-05-16-19-06-25-dvl-drr.csv'
    vrPath = r'C:\Users\Meissa Vaccines\Downloads\Cyclone_VideoFeed\2026-05-16-19-06-25-dvl-vr.csv'

    # open video capture
    cap = cv2.VideoCapture(videoPath)

    if not cap.isOpened():
        raise IOError(f"Could not open video: {videoPath}")

    # open csv file
    df_drr = pd.read_csv(drrPath)
    df_vr = pd.read_csv(vrPath)

    # parse the time column ("2026-05-16-19-06-39") into epoch seconds
    df_drr["timeSeconds"] = (
        pd.to_datetime(df_drr["time"], format="%Y-%m-%d-%H-%M-%S").astype("int64") // 10**9
    )
    df_vr["timeSeconds"] = (
        pd.to_datetime(df_vr["time"], format="%Y-%m-%d-%H-%M-%S").astype("int64") // 10**9
    )

    # average all altitude readings that share the same second
    df_vr_avg = (
        df_vr.groupby("timeSeconds")["altitude"]
        .mean()
        .reset_index()
        .rename(columns={"altitude": "altitude_avg"})
    )

    # align the video's t=0 with the first timestamp in the csv
    startingTimestamp = df_drr["timeSeconds"].iloc[0]

    #resize video
    cv2.namedWindow("video",cv2.WINDOW_NORMAL)
    cv2.resizeWindow("video",800,600)
    # while video is open
    text = "video"
    while cap.isOpened():
        ret, frame = cap.read()
        height, width, _ = frame.shape
        if not ret:
            break

        # get timestamp of the frame
        frameTimestamp = startingTimestamp + cap.get(cv2.CAP_PROP_POS_MSEC) / 1000.0

        # if timestamp match any timestamp from csv (1-second resolution)
        matches = df_drr[df_drr["timeSeconds"] == int(frameTimestamp)]

        for _, row in matches.iterrows():
            # positions x, y, and z from the current iteration n correlating to the matched timestamp
            # add the change in degrees from iter n to the origin latitude and longitude to get the updated lat and long
            new_lat, new_long = offset_to_latlon(
                row["position_x"], row["position_y"],
                lat_o, long_o, radius,
                heading_deg,
            )
            time_str = row["time"][-8:]
            left_lines = [
                f"Time: {time_str}",
                f"Latitude: {new_lat:.6f}",
                f"Longitude: {new_long:.6f}",
            ]

        # look up the averaged altitude for this second
        vr_match = df_vr_avg[df_vr_avg["timeSeconds"] == int(frameTimestamp)]
        if not vr_match.empty:
            avg_altitude = vr_match["altitude_avg"].iloc[0]
            right_text = f"Altitude: {avg_altitude:.3f} m"

        # left text appearance
        font = cv2.FONT_HERSHEY_DUPLEX
        line_height = 35
        for i, line in enumerate(left_lines):
            cv2.putText(
                frame,
                line,
                (50, 50 + i * line_height),
                font,
                1.0,
                (0, 255, 0),
                2,
                cv2.LINE_AA,
            )

        # right text appearance
        org_right = (frame.shape[1]-350,50)
        cv2.putText(frame, right_text, org_right, font, 1.0, (0, 255, 0), 2)

        cv2.imshow("video", frame)
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()