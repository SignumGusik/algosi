import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

PI = 3.141592653589793
S_exact = 0.25 * PI + 1.25 * np.arcsin(0.8) - 1.0
df = pd.read_csv("data.csv")

N = df["N"]
S_wide = df["S_wide"]
S_narrow = df["S_narrow"]
err_wide = df["err_wide"]
err_narrow = df["err_narrow"]

plt.figure(figsize=(10, 5))
plt.plot(N, S_wide, label="S_wide")
plt.plot(N, S_narrow, label="S_narrow")
plt.axhline(S_exact, color="black", linestyle="--", label="S_exact")

plt.xlabel("N")
plt.ylabel("Оценка площади")
plt.title("Оценка площади пересечения трёх кругов")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(N, err_wide, label="err_wide")
plt.plot(N, err_narrow, label="err_narrow")

plt.xlabel("N")
plt.ylabel("Относительная ошибка")
plt.title("Относительная ошибка оценки площади")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
