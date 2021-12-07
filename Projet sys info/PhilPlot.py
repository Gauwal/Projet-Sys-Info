import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

data=pd.read_csv("Tache1/phil.csv")
data2=pd.read_csv("Tache2/philaa.csv")
data3=pd.read_csv("Tache2/philaa_tatas.csv")

a=[data["1"],data["2"],data["3"],data["4"],data["5"],data["6"]]
b=[data2["1"],data2["2"],data2["3"],data2["4"],data2["5"],data2["6"]]
c=[data3["1"],data3["2"],data3["3"],data3["4"],data3["5"],data3["6"]]

y=[np.mean(i) for i in a]
er=[np.std(i) for i in a]
y2=[np.mean(i) for i in b]
er2=[np.std(i) for i in b]
y3=[np.mean(i) for i in c]
er3=[np.std(i) for i in c]

fig1 = plt.figure()

x = [2*i for i in range(1,7)]

plt.errorbar(x, y,er, color="blue", capsize=5, linewidth=1.0, linestyle="-", label="Sémaphores POSIX")
plt.errorbar(x, y2,er2, color="red", capsize=5, linewidth=1.0, linestyle="-", label="Attende active TaS")
plt.errorbar(x, y3,er3, color="green", capsize=5, linewidth=1.0, linestyle="-", label="Attende active TaTas")

plt.xlabel("Nombre de philosophes")
plt.ylabel("Temps(s)")
plt.title("Philosophes")
plt.ylim(0,max(y2)+max(er2))
plt.xlim(0,13)
plt.legend(loc="upper left")
         
plt.show()
