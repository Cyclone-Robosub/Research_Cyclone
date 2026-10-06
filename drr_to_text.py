import cv2
import pandas as pd

#files
drr = pd.read_csv(r'C:\Users\Meissa Vaccines\Downloads\Cyclone_VideoFeed\2026-05-16-19-06-25-dvl-drr.csv')
vr = pd.read_csv(r'C:\Users\Meissa Vaccines\Downloads\Cyclone_VideoFeed\2026-05-16-19-06-25-dvl-vr.csv')
feed = r'D:\output_20260516_190623_front.mp4'

# Load the video file
cap = cv2.VideoCapture(feed)

# Check if the video opened correctly
if not cap.isOpened():
    print('Error: Could not open video file.')
    exit()

# Read and display video frames
while True:
    ret, frame = cap.read()

    if not ret:
        break   # No more frames -> exit loop

    cv2.imshow("Video", frame)

    # Press Q to quit
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break


