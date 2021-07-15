import cv2
import numpy as np
from io import BytesIO
from tqdm import trange

filename_video = "ba.flv"
filename_out = "ba.bin"
shape_out = (15, 7)
frame_skip = 0

def threshold_point(point: np.ndarray, th: int = 127) -> int:
    if (int(point[0]) + int(point[1]) +int( point[2])) / 3 > th:
        return 1
    return 0

def parse_frame(frame: np.ndarray) -> list:
    frame_data = [[0x00 for _ in range(5)] for __ in range(3)]
    for i in range(3):
        for y in range(7):
            for x in range(5):
                if threshold_point(frame[y][i * 5 + x]):
                    frame_data[i][x] += (1 << y)
    return frame_data

def main():
    cap = cv2.VideoCapture(filename_video)
    frame_count = 0
    while True:
        is_success, frame = False, None
        for i in range(frame_skip + 1):
            is_success, frame = cap.read()
        if not is_success:
            break
        frame_count += 1
    cap = cv2.VideoCapture(filename_video)
    f = open(filename_out, "wb")
    data = BytesIO()
    for _ in trange(frame_count):
        is_success, frame = False, None
        for i in range(frame_skip + 1):
            is_success, frame = cap.read()
        if not is_success:
            break
        frame = cv2.resize(frame, shape_out)
        # cv2.imshow("frame", frame)
        # cv2.waitKey(1)
        frame_data = parse_frame(frame)
        for k in range(3):
            for i in range(5):
                # print("0x%02X" % frame_data[k][i])
                data.write(int(frame_data[k][i]).to_bytes(length=1, byteorder='big', signed=False))
    size = data.tell()
    data.seek(0)
    f.write(data.read())
    f.close()
    print("done with", size, "bytes")


if __name__ == '__main__':
    main()
