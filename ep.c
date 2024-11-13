/** @file ep.c
 * @brief Para uso como exemplo, contém uma aplicação da estratégia gulosa para
 * solução do TSP. Para uso como modelo de EP, deve conter sua solução para o EP.
 * 
 * Lembre-se de substituir o nome do autor, abaixo, pelo seu nome e NUSP.
 * (Redundância com o retorno da função autor())
 * 
 * @author Fábio Nakano NUSP 1048221
 */
#include "ep.h"
#if 0 // modelo
Conjunto Guloso(Conjunto C)
/∗ C: conjunto de candidatos ∗/
{
	S = ∅ ; /∗ S contem conjunto solucao ∗/
	while( (C ! = ∅) && !( solucao(S) ) ) {
		x = seleciona (C) ;
		C = C − x;
		if (viavel (S + x )) S = S + x ;
	}
	if solucao(S) return(S) else return( ’Nao existe solucao ’ ) ;
}
#endif

int *grauDoVertice;
int *arestaUsada;  // flag que informa se uma aresta está sendo usada no caminho que está sendo construído ou não. Ao final, se o algoritmo chegar a uma solução, este array conterá essa solução.

/** @brief Esta função é importante para que o corretor automático inclua 
 * corretamente a autoria do EP. Deve retornar Nome e NUSP separados por 
 * ponto-e-virgula, sem pular linhas no meio nem no final.
 */
char *autor (void) {
	return "Fábio Nakano ; 1048221";
}

int ehSolucao (struct grafo *G) { // devido à maneira de construção da solução, verificar que todos os vértices foram alcançados NÃO equivale à existência de um caminho hamiltoniano.
// tem que testar o grau do vértice
// se o grafo for composto por vários circuitos disjuntos que cobrem o grafo 
// todo, o que não é uma solução, os graus serão todos 2 e esta função 
// retornará que ehSolucao. Esta situação é evitada pela maneira como o circuito
// é construido pela função tenta, que vai aumentando um (único) caminho.
# if DEBUG
	for (int i=0;i<G->N;i++) printf ("%d ", grauDoVertice[i]);
    puts("");
# endif
	for (int i=0;i<G->N;i++) {
		if (grauDoVertice[i]!=2) return 0; // não coberto ou completou circuito com vértice errado. 
	}
	return 1;
}

int ehAceitavel (struct grafo *G, int arestaATestar, int verticeAtual) {
	if (arestaUsada[arestaATestar]) return 0;   // aresta a testar está em uso. Não é aceitável
	if (G->A[3*arestaATestar] == verticeAtual) { // arestaATestar conecta no vértice atual
		if (grauDoVertice[G->A[3*arestaATestar+1]] < 2) { // vértice no outro extremo da aresta não é coberto.
			return 1; // é aceitável
		}
	}
	if (G->A[3*arestaATestar+1] == verticeAtual) { // arestaATestar conecta no vértice atual.
		if (grauDoVertice[G->A[3*arestaATestar]] < 2) { // vértice no outro extremo da aresta não é coberto.
			return 1; // é aceitável
		}
	}
	return 0;
}

/* Parte do pressuposto que arestaAcrescentar é aceitável. */
int aumentaCaminho (struct grafo *G, int arestaAcrescentar, int verticeAtual) {  // registra movimento
	arestaUsada[arestaAcrescentar]=1;   // TRUE: agora está em uso.
	grauDoVertice[G->A[3*arestaAcrescentar+1]]++;
	grauDoVertice[G->A[3*arestaAcrescentar]]++;
	if (G->A[3*arestaAcrescentar] == verticeAtual) { // ajusta o lado
		return G->A[3*arestaAcrescentar+1];
	}
	return G->A[3*arestaAcrescentar];
}

int guloso(struct grafo *G, int verticeAtual ) {
  /* inicializa seleção de movimentos; */
  int iAresta=0;
  while ((iAresta<G->M)&&(!ehSolucao (G))) { 
	/* x = seleciona (C) ; */
	
    if ( ehAceitavel (G, iAresta, verticeAtual) ) {  //if (viavel (S + x )) 
		/* registra movimento; */ 
		verticeAtual=aumentaCaminho (G, iAresta, verticeAtual);  //  S = S + x ;
    }
    iAresta++; // C = C − x;
  } 
  if (ehSolucao(G)) {
	puts ("Achou solução");
	return 1;
  }
  puts ("Não achou solução");
  return 0;
}

int iniciaEexecuta(struct grafo *G, int verticeInicial) {
    grauDoVertice=calloc(G->N, sizeof(int)); // necessário inicializar com zeros;
    arestaUsada=calloc(G->M, sizeof(int));    // necessário inicializar com zeros;
// como o circuito Hamiltoniano inclui todos os vértices do grafo, tanto faz por onde começa. 	
    int r=guloso(G,verticeInicial);
	for (int i=0;i<G->M;i++) {  /**< ATENÇÃO: TEM QUE TER ESTA PARTE PARA A RESPOSTA PODER SER CORRIGIDA!! */
	// como arestausada não é restaurada no retorno da recursão, quando acha o circuito, posso passar para o imprimeMermaid.
		G->S[i]=arestaUsada[i];
	}
	return r;
}

void termina() {
	free (grauDoVertice);
	free (arestaUsada);
}


