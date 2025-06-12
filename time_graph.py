import pandas as pd
import matplotlib.pyplot as plt


res_filepath = "./generation_time.csv"
res_picpath  = "./generation_time.png"


results = pd.read_csv(res_filepath)

names = results.columns[1:]


plt.figure(figsize=(10,10),layout="tight")



plt.subplot(211)

plt.title("Зависимость времени генерации от объёма выборки для различных методов")

for name in names:
  plt.plot(results["Size"], results[name], "--o")
  
plt.grid(linestyle=":")
plt.legend(results.columns[1:])

plt.xlabel("Количесто элементов")
plt.ylabel("Время генерации, µs")


plt.subplot(212)

for name in names:
  plt.plot(results["Size"], results[name], "--o")
  
plt.grid(linestyle=":")
plt.legend(results.columns[1:])

plt.xlabel("Количесто элементов")
plt.ylabel("Время генерации, µs")

plt.yscale("log")


plt.savefig(res_picpath)