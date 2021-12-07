PY = python3

RunTache1:
	cd Tache1 && $(MAKE) All
RunTache2:
	cd Tache2 && $(MAKE) All

GraphAll: GraphPhil GraphProd GraphRead GraphTest

GraphPhil: PhilPlot.py
	$(PY) PhilPlot.py

GraphProd: ProdPlot.py
	$(PY) ProdPlot.py

GraphRead: ReadPlot.py
	$(PY) ReadPlot.py

GraphTest: TestAndSetPlot.py
	$(PY) TestAndSetPlot.py

Clean: 
	cd tache1 && $(MAKE) Clean
	cd tache2 && $(MAKE) Clean
