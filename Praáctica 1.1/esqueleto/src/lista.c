#include <stdio.h>
#include <stdlib.h>
#include <lista.h>

// Crea una lista con un nodo.
void crear(TLista *pLista, int valor)
{
  pLista->pPrimero = malloc(sizeof(TNodo));
  pLista->pPrimero->valor = valor;
  pLista->pPrimero->pSiguiente = NULL;
}

void destruir(TLista *pLista)
{
  TNodo *pAux1;
  struct Nodo *pAux2;

  for (pAux1 = pLista->pPrimero; pAux1 != NULL;)
  {
    pAux2 = pAux1->pSiguiente;
    free(pAux1);
    pAux1 = pAux2;
  }

  free(pLista);
}

// Inserta al ppio de la lista.
void insertar(TLista *pLista, int valor)
{
  // TODO
  TNodo *Nuevo = malloc(sizeof(TNodo));
  Nuevo->valor=valor;
  Nuevo->pSiguiente=pLista->pPrimero;
  pLista->pPrimero=Nuevo;
  //pLista->pPrimero->pSigueinte=pAux1;

}

void insertarFinal(TLista *pLista, int valor)
{
  // TODO0
  TNodo *Nuevo = malloc(sizeof(TNodo));
  Nuevo->valor=valor;
  TNodo *Paux;
  TNodo *Paux1;

  for (Paux = pLista->pPrimero){
    Paux1 = Paux->pSiguiente;
    Paux = Paux1;
    if(Paux==NULL){
      
    }
  }
}

// Suponemos n = 1, 2, ...
void insertarN(TLista *pLista, int index, int valor)
{
  // TODO

}

// Elimina el primer elemento de la lista.
void eliminar(TLista *pLista)
{
  // TODO
}

void eliminarN(TLista *pLista, int index)
{
  // TODO
}

int getElementoN(TLista *pLista, int index)
{
  // TODO
  return 0;
}

void imprimir(TLista *pLista)
{
  // TODO
}

int longitud(TLista *pLista)
{
  // TODO
  return -1;
}