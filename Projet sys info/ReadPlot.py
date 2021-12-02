import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

data=pd.read_csv("ReadWrite.csv")

a=[data["1"],data["2"],data["3"],data["4"],data["5"],data["6"]]
y=[np.mean(i) for i in a]
er=[np.std(i) for i in a]

fig1 = plt.figure()

x = [2*i for i in range(1,7)]

plt.errorbar(x, y,er, color="blue", capsize=5, linewidth=1.0, linestyle="-")

plt.xlabel("Nombre de threads")
plt.ylabel("Temps(s)")
plt.title("ReaderWriter")
plt.ylim(0,max(y)+max(er)*2)
plt.xlim(0,13)
         
plt.show()
