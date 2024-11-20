/** @file ep.c
 * @brief Para uso como exemplo, contém uma aplicação da estratégia gulosa para
 * solução do TSP. Para uso como modelo de EP, deve conter sua solução para o EP.
 * 
 * Lembre-se de substituir o nome do autor, abaixo, pelo seu nome e NUSP.
 * (Redundância com o retorno da função autor())
 * 
 * @author Alan Moura NUSP 15436668
 */
#include "ep.h"
#include "limits.h"
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

int caminhoPeso = INT_MAX;
int *grauDoVertice;
int *arestaUsada, *arestaSolucao;
int *visitado;
int verticeIni;
 // flag que informa se uma aresta está sendo usada no caminho que está sendo construído ou não. Ao final, se o algoritmo chegar a uma solução, este array conterá essa solução.

/** @brief Esta função é importante para que o corretor automático inclua 
 * corretamente a autoria do EP. Deve retornar Nome e NUSP separados por 
 * ponto-e-virgula, sem pular linhas no meio nem no final.
 */
char *autor (void) {
	return "Alan Moura ; 15436668";
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
		//if (grauDoVertice[G->A[3*arestaATestar+1]] < 2) { // vértice no outro extremo da aresta não é coberto.
			return 1; // é aceitável
		//}
	}
	if (G->A[3*arestaATestar+1] == verticeAtual) { // arestaATestar conecta no vértice atual.
		//if (grauDoVertice[G->A[3*arestaATestar]] < 2) { // vértice no outro extremo da aresta não é coberto.
			return 1; // é aceitável
		//}
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

void diminuiCaminho (struct grafo *G, int arestaAcrescentar) {  
	arestaUsada[arestaAcrescentar]=0; 
	grauDoVertice[G->A[3*arestaAcrescentar+1]]--;
	grauDoVertice[G->A[3*arestaAcrescentar]]--;
}


int melhorArestaFinal(struct grafo *G, int verticeAtual){

	int peso = INT_MAX;
	int arestaFinal = -1;
	int j;
	for (j = 0; j < G->M; j++) {
		if ((G->A[3 * j] == verticeAtual && G->A[3 * j + 1] == verticeIni) ||
        (G->A[3 * j + 1] == verticeAtual && G->A[3 * j] == verticeIni)) {

			if(G->A[3 * j + 2] < peso){
				peso = G->A[3 * j + 2];
				arestaFinal = j;
			}

		}
	}
	return arestaFinal;
}


int backtracking(struct grafo *G, int nivel, int verticeAtual) {

	//printf("Nova chamada");
	//printf(" Vertice atual: %d ", verticeAtual);
	//printf(" Nivel: %d \n", nivel);

    if (nivel == G->N) {

		int fechaCiclo = melhorArestaFinal(G, verticeAtual);

		if(fechaCiclo != -1){

			arestaUsada[fechaCiclo] = 1;

                int caminhoteste = 0;
                for (int k = 0; k < G->M; k++) {
                    if (arestaUsada[k] == 1) caminhoteste += G->A[3 * k + 2];
                }

                if (caminhoteste < caminhoPeso){ caminhoPeso = caminhoteste;
					//printf("\n");
					for (int k = 0; k < G->M; k++) {
						arestaSolucao[k] = arestaUsada[k]; 
						//printf(" %d ", arestaSolucao[k]);
					}
					//printf("\n");
				}else{
					arestaUsada[fechaCiclo] = 0;
				}

                return 1;

		}
			return 0;
    }

    int encontrouSolucao = 0;
    for (int i = 0; i < G->M; i++) {
        int proximoVertice = -1;

        if (G->A[3 * i] == verticeAtual && !visitado[G->A[3 * i + 1]]) {
            proximoVertice = G->A[3 * i + 1];
        } else if (G->A[3 * i + 1] == verticeAtual && !visitado[G->A[3 * i]]) {
            proximoVertice = G->A[3 * i];
        }

        if (proximoVertice != -1 && ehAceitavel(G, i, verticeAtual)) {

			//printf("Aresta usada: %d \n", i);

            aumentaCaminho(G, i, verticeAtual);       // Marca aresta como usada
            visitado[proximoVertice] = 1;             // Marca o vértice como visitado
            encontrouSolucao |= backtracking(G, nivel + 1, proximoVertice);
			//diminuiCaminho(G,i);
			//printf("Aresta desusada: %d \n", i);
			//printf("Vertice desvisitado: %d \n", proximoVertice);
            visitado[proximoVertice] = 0;             // Desmarca o vértice
            arestaUsada[i] = 0;                       // Desmarca a aresta
        }
    }

    return encontrouSolucao;
}

int iniciaEexecuta(struct grafo *G, int verticeInicial) {
    grauDoVertice=calloc(G->N, sizeof(int)); // necessário inicializar com zeros;
    arestaUsada=calloc(G->M, sizeof(int));
	arestaSolucao=calloc(G->M, sizeof(int)); // necessário inicializar com zeros;
	verticeIni = verticeInicial;
	visitado = calloc(G->N, sizeof(int));
// como o circuito Hamiltoniano inclui todos os vértices do grafo, tanto faz por onde começa. 	
	visitado[verticeIni] = 1;
    int r=backtracking(G, 1, verticeIni);
	for (int i=0;i<G->M;i++) {  /**< ATENÇÃO: TEM QUE TER ESTA PARTE PARA A RESPOSTA PODER SER CORRIGIDA!! */
	// como arestausada não é restaurada no retorno da recursão, quando acha o circuito, posso passar para o imprimeMermaid.
		G->S[i]=arestaSolucao[i];
	}
	return r;
}

void termina() {
	free (grauDoVertice);
	free (arestaUsada);
}


