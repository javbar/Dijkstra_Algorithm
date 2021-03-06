#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

#include "City.h"
#include "List.h"

class Grafo
{
public:
	List data;
	City **solution;
	City *city;
	int cityN;
	int solutionStepts;

	Grafo();
	~Grafo();

	void printGrafo();
	void connectCities();
	void dijkstraAlgorithm(City *origin, City *destination);
	City *calculateDistances(City *actual, City *destination);
	void getSolution(City *actual);
	bool searchValues(char *origin, char *destination);

	bool cityExists(char *origin);
	bool connectionExists(char *origen, char *destino);
	void updateCities();
};

Grafo::Grafo()
{
	city = new City[50];
	cityN = 0;
	solutionStepts = 0;
	connectCities();
}

Grafo::~Grafo()
{
	for (int i = 0; i < cityN; i++)
		if (city[i].numAdy != 0)
			delete[] city[i].adyacent;

	delete[] solution;
	delete[] city;
}

void Grafo::connectCities()
{
	/********************* CREAR CIUDADES COMO "NODOS" ************************/
	bool repeated;
	char name[50];
	Node *actual;

	actual = data.first;

	strcpy(city[cityN].name, actual->origin);
	city[cityN].ID = cityN;
	cityN++;

	while (actual != NULL)
	{
		repeated = false;
		strcpy(name, actual->origin);

		for (int i = 0; i < cityN; i++)
			if (strcmp(city[i].name, name) == 0)
			{
			repeated = true;
			break;
			}

		if (!repeated)
		{
			strcpy(city[cityN].name, name);
			city[cityN].ID = cityN;
			cityN++;
		}

		repeated = false;
		strcpy(name, actual->destiny);

		for (int i = 0; i < cityN; i++)
			if (strcmp(city[i].name, name) == 0)
			{
			repeated = true;
			break;
			}

		if (!repeated)
		{
			strcpy(city[cityN].name, name);
			city[cityN].ID = cityN;
			cityN++;
		}

		actual = actual->next;
	}

	solution = new City*[cityN];

	/********************* CONTAR ADYACENCIAS ************************/
	for (int i = 0; i < cityN; i++)
	{
		actual = data.first;

		while (actual != NULL)
		{
			if (strcmp(city[i].name, actual->origin) == 0)
				city[i].numAdy++;

			if (strcmp(city[i].name, actual->destiny) == 0)
				city[i].numAdy++;

			actual = actual->next;
		}
	}

	/********************* ASIGNACION DE MEMORIA A ADYACENCIAS ************************/
	for (int i = 0; i < cityN; i++)
		city[i].adyacent = new City*[city[i].numAdy];

	/********************* CONEXIONES ENTRE ADYACENCIAS ************************/
	actual = data.first;
	int ID;

	for (int i = 0; i < cityN; i++)
		city[i].numAdy = 0;

	while (actual != NULL)
	{
		for (int i = 0; i < cityN; i++)
		{
			if (strcmp(city[i].name, actual->origin) == 0)
			{
				for (int j = 0; j < cityN; j++)
					if (strcmp(city[j].name, actual->destiny) == 0)
						ID = j;

				city[i].adyacent[city[i].numAdy++] = &city[ID];
				city[ID].adyacent[city[ID].numAdy++] = &city[i];
			}
		}
		actual = actual->next;
	}

}

void Grafo::printGrafo()
{
	for (int i = 0; i < cityN; i++)
		city[i].printCity();
}

void Grafo::dijkstraAlgorithm(City *origin, City *destination)
{
	/* LIMPIA DISTANCIAS EN CASO DE VOLVER A USAR ALGORITMO AL IGUAL QUE LOS VISITADOS */
	printGrafo();

	solutionStepts = 0;
	for (int i = 0; i < cityN; i++)
	{
		city[i].distance = 1000000;
		city[i].visited = false;
	}
	/************************************************************************************/

	origin->distance = 0;
	calculateDistances(origin, destination);

	cout << "Ruta mas corta:" << endl;

	getSolution(destination);
}

City *Grafo::calculateDistances(City *actual, City *destination)
{
	int newDistance;
	actual->visited = true;
	newDistance = actual->distance;

	for (int i = 0; i < actual->numAdy; i++)
	{
		if (!actual->adyacent[i]->visited)
		{
			newDistance = data.getCosto(actual->name, actual->adyacent[i]->name);
			newDistance += actual->distance;

			if (newDistance < actual->adyacent[i]->distance)
				actual->adyacent[i]->distance = newDistance;
		}
	}

	newDistance = 1000;
	int aux = 0;
	for (int i = 0; i < cityN; i++)
	{
		if (!city[i].visited && city[i].distance < newDistance)
		{
			newDistance = city[i].distance;
			aux = i;
		}
	}

	actual = &city[aux];

	if (actual == destination)
		return actual;

	else
		calculateDistances(actual, destination);
}

void Grafo::getSolution(City *actual)
{
	while (actual->distance != 0)
		for (int i = 0; i<actual->numAdy; i++)
		{
			if ((actual->distance - data.getCosto(actual->name, actual->adyacent[i]->name)) == actual->adyacent[i]->distance)
			{
				solution[solutionStepts] = actual;
				actual = actual->adyacent[i];
				solutionStepts++;
				break;
			}
		}

	solution[solutionStepts] = actual;
	solutionStepts++;

	for (int i = 0; i < solutionStepts; i++)
		cout << solution[solutionStepts - i - 1]->name << endl;
}

bool Grafo::searchValues(char *origen, char *destino)
{
	City *origin, *destination;
	origin = NULL;
	destination = NULL;

	for (int i = 0; i < cityN; i++)
	{
		if (strcmp(city[i].name, origen) == 0)
			origin = &city[i];

		if (strcmp(city[i].name, destino) == 0)
			destination = &city[i];
	}
	
	if (origin == NULL || destination == NULL)
		return false;

	else
	{
		cout << "\nMostrando Mapa con Adyacencias:" << endl << endl;
		dijkstraAlgorithm(origin, destination);
		return true;
	}
}

bool Grafo::cityExists(char *origin)
{
	for (int i = 0; i < cityN; i++)
		if (strcmp(city[i].name, origin) == 0)
			return true;

	return false;
}

bool Grafo::connectionExists(char *origen, char *destino)
{
	Node *actual;
	actual = data.first;

	while (actual != NULL)
	{
		if (strcmp(actual->origin, origen) == 0 && strcmp(actual->destiny, destino) == 0)
			return true;

		if (strcmp(actual->origin, destino) == 0 && strcmp(actual->destiny, origen) == 0)
			return true;

		actual = actual->next;
	}

	return false;
}

void Grafo::updateCities()
{
	for (int i = 0; i < cityN; i++)
	{
		city[i].numAdy = 0;
		delete[] city[i].adyacent;
	}
	cityN = 0;
	connectCities();
}
#endif
