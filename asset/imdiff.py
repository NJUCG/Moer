# Display two images, diff and evaluate rMSE and relMSE
# Usage: python imdiff.py TextForOurs our_result.bmp TextForGT ground_truth.png

import numpy as np
from matplotlib import pyplot as plt
from sklearn.metrics import mean_squared_error
import sys

if len(sys.argv) < 5:
    print("Invalid arguments")
    print("Usage: python imdiff.py TextForOurs our_result.bmp TextForGT ground_truth.png")
    exit(1)

title_our_output = sys.argv[1]
filename_our_output = sys.argv[2]
title_ground_truth = sys.argv[3]
filename_ground_truth = sys.argv[4]

img_out = plt.imread(filename_our_output).astype(np.float32)/255
img_out = np.array([[np.average(j)
                   for j in i] for i in img_out])
img_ans = plt.imread(filename_ground_truth).astype(np.float32)
img_ans = np.array([[np.average(j)
                   for j in i] for i in img_ans])
img_delta = (img_out - img_ans)
img_delta = np.array([[np.average(j) for j in i] for i in img_delta])
img_rel = img_delta / (np.maximum(img_out, img_ans) + 1e-6)
relmse = np.sqrt(np.average([j**2 for i in img_rel for j in i]))

plt.subplot(221)
plt.title(title_our_output)
plt.imshow(img_out, cmap='gray', vmin=0.0, vmax=1.0)
plt.colorbar()
plt.subplot(222)
plt.title(title_ground_truth)
plt.imshow(img_ans, cmap='gray', vmin=0.0, vmax=1.0)
plt.colorbar()
plt.subplot(223)
plt.title("Error")
plt.imshow(img_delta, cmap='coolwarm', vmin=-0.1, vmax=0.1)
plt.colorbar()
plt.text(
    8, 12, f"rMSE={int(np.sqrt(mean_squared_error(img_out, img_ans))*10000)/10000}")
plt.subplot(224)
plt.title("Relative Error")
plt.imshow(img_rel, cmap='coolwarm', vmin=-0.5, vmax=0.5)
plt.colorbar()
plt.text(
    8, 12, f"relMSE={int(relmse*10000)/10000}")

plt.show()
