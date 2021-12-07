import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

data=pd.read_csv("Tache2/Test_and_set.csv")

a=[data["1"],data["2"],data["3"],data["4"],data["5"],data["6"],data["7"]]
y=[np.mean(i) for i in a]
er=[np.std(i) for i in a]

fig1 = plt.figure()

x = [2*i for i in range(1,8)]

plt.errorbar(x, y,er, color="blue", capsize=5, linewidth=1.0, linestyle="-", label="Test_and_set")

plt.xlabel("Nombre de threads")
plt.ylabel("Temps(s)")
plt.title("Threads")
plt.ylim(0,max(y)+max(er))
plt.xlim(0,15)
         
         
data=pd.read_csv("Tache2/Test_and_Test_and_set.csv")

a=[data["1"],data["2"],data["3"],data["4"],data["5"],data["6"],data["7"]]
y=[np.mean(i) for i in a]
er=[np.std(i) for i in a]



plt.errorbar(x, y,er, color="red", capsize=5, linewidth=1.0, linestyle="-", label="Test_and_Test_and_set")

plt.legend(loc="upper left")

plt.savefig("Graphes/Test_and_set.png")
