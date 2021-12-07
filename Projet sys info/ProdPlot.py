import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

data=pd.read_csv("Tache1/Prod.csv")
data2=pd.read_csv("Tache2/Prodaa.csv")

a=[data["1"],data["2"],data["3"],data["4"],data["5"],data["6"]]
b=[data2["1"],data2["2"],data2["3"],data2["4"],data2["5"],data2["6"]]
y=[np.mean(i) for i in a]
er=[np.std(i) for i in a]
y2=[np.mean(i) for i in b]
er2=[np.std(i) for i in b]

fig1 = plt.figure()

x = [2*i for i in range(1,7)]

plt.errorbar(x, y,er, color="blue", capsize=5, linewidth=1.0, linestyle="-", label="Sémaphores POSIX")
plt.errorbar(x, y2,er2, color="red", capsize=5, linewidth=1.0, linestyle="-", label="Sémaphores attende active")


plt.xlabel("Nombre de threads")
plt.ylabel("Temps(s)")
plt.title("Producteurs-consomateurs")
plt.ylim(0,max(y)+max(er)*2)
plt.xlim(0,13)
         
plt.show()
plt.close()
