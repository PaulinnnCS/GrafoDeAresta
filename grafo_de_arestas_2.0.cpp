#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// complexidade: O(N^5)

// variaveis de controle (indice): i, j, c, d

// regsitro que armazena quais vertices compoem o novo vertice
typedef struct{
	int x, y;
} vertice;

// FUNCOES ============================================================================================================================================================================================================

FILE * abrir_arquivo(char * nome_arquivo);						// funcao para abrir o arquivo
int ** ler_arquivo(FILE * arquivo);							// retorna a matriz adjacencia do arquivo
int tamanho_matriz(FILE * arquivo);							// retorna o tamanho da matriz adjacencia do arquivo
int quantidade_vertice_grafo_aresta(int ** matriz_adjacencia, FILE * arquivo);		// retorna a quantidade de vertices que o grafo de aresta terá
vertice * formar_vertices_grafo_aresta(int ** matriz_adjacencia, FILE * arquivo);	// forma os vertices do grafo de aresta
void ler_grafo_aresta(vertice * vertice_grafo_aresta, int qnt_vertice_grafo_aresta);    // faz a leitura do grafo de aresta

// MAIN ===========================================================================================================================================================================================================================

int main(){
	char * nome_arquivo; 			// nome do arquivo de entrada
	FILE * arquivo;      			// ponteiro do arquivo
	int ** matriz_adjacencia; 		// armazena a matriz de adjacencia de entrada
	int qnt_vertice_grafo_aresta; 	// quantidade de vertices que o grafo de arestas terá
	
	// aloca memoria para nome_arquivo
	nome_arquivo = (char *) malloc(sizeof(char) * 80);
	
	printf("Digite o nome do arquivo ou S para sair: ");
	scanf("%s", nome_arquivo);
	
	// verifica se o usuario deseja sair 
	while(strcmp(nome_arquivo, "S") != 0 && strcmp(nome_arquivo, "s") != 0){ // O(N) * O(N^4) = O(N^5)
		
		// abre o arquivo e armazena seu endereco no ponteiro arquivo
		arquivo = abrir_arquivo(nome_arquivo);
		
		// leitura do arquivo que eh armazenada na matriz
		matriz_adjacencia = ler_arquivo(arquivo); // O(N²)
		
		// armazena a quantidade de vertice do grafo de aresta 
		qnt_vertice_grafo_aresta = quantidade_vertice_grafo_aresta(matriz_adjacencia, arquivo); // O(N²)
		
		// se a quantidade for zero, o grafo de entrada nao tem arestas
		if(qnt_vertice_grafo_aresta == 0){
			printf("Grafo sem arestas, portanto nao existe um grafo de arestas para esse grafo");
		}
		
		// leitura da matriz de adjacencia do grafo de aresta a partir dos vertices formados e a quantidade de vertice do grafo de aresta
		ler_grafo_aresta(formar_vertices_grafo_aresta(matriz_adjacencia, arquivo), qnt_vertice_grafo_aresta); // O(N²) * O(N²) = O(N^4)
		
		system("pause");
		system("cls");
		
		printf("Digite o nome do arquivo ou S para sair: ");
		scanf("%s", nome_arquivo);
	}
	
	printf("Programa finalizado.\n\n");
	
	free(nome_arquivo);
	free(matriz_adjacencia);
	
	return 0;
}

// ABRE O ARQUIVO ===============================================================================================================================================================

FILE * abrir_arquivo(char * nome_arquivo){
	FILE * arquivo;
	
	// concatena o nome do arquivo com a extensao do arquivo txt
	strcat(nome_arquivo, ".txt");
	
	// abre o arquivo para leitura
	arquivo = fopen(nome_arquivo, "r");
	
	system("cls");
	
	// Verifica se o arquivo foi ou não encontrado
	if(arquivo == NULL){
		printf("Arquivo inexistente, tente novamente!\n");
		system("pause");
		system("cls");
	}else{
		printf("Arquivo localizado com sucesso! \n");
		system("pause");
		system("cls");
		
		return arquivo;
	}
}

// TAMANHO DA MATRIZ ADJACENCIA DE ENTRADA ===========================================================================================================================================

int tamanho_matriz(FILE * arquivo){ // O(N)
	char caractere;
	int contador_tamanho_matriz = 1;
	caractere = fgetc(arquivo);
	
	while(caractere != EOF){ // O(N)
		if(caractere == '\n'){
			contador_tamanho_matriz++;
		}
		caractere = fgetc(arquivo);
	}
	
	// reposiciona o ponteiro do arquivo para o inicio
	fseek(arquivo, 0, SEEK_SET);

	return contador_tamanho_matriz;
}

// FAZ A LEITURA DO ARQUIVO E RETORNA A MATRIZ DE ENTRADA =========================================================================================================================

int ** ler_arquivo(FILE * arquivo){ // O(N) + O(N) + O(N²) = O(N²)
	char caractere;
	int contador_tamanho_matriz = tamanho_matriz(arquivo); // O(N)
	int ** matriz_adjacencia;
	int i = 0;
	int j = 0;

	// aloca memoria matriz_adjacencia
	matriz_adjacencia = (int **) malloc(sizeof(int *) * contador_tamanho_matriz);
	for(int c = 0; c < contador_tamanho_matriz; c++){ // O(N)
		matriz_adjacencia[c] = (int *) malloc(sizeof(int) * contador_tamanho_matriz);
	}
	
	// le um caractere do arquivo
	caractere = fgetc(arquivo);
	
	// enquanto o caractere seja diferente de "fim do arquivo", o loop continua
	while(caractere != EOF){ // O(N)
		// enquanto o caractere seja diferente de "quebra de linha" e "fim do arquivo", o loop continua
		while(caractere != '\n' && caractere != EOF){ // O(N)
			// verifica se o carcatere eh um caractere valido para a matriz
			if(caractere != ' ' && caractere != '\n' && caractere != EOF){
				matriz_adjacencia[i][j] = caractere - '0';
				j++;	
			}
			// continua lendo caracteres do arquivo, um por um
			caractere = fgetc(arquivo);
		}
		
		caractere = fgetc(arquivo);
		i++;
		j = 0;
	}
	
	return matriz_adjacencia;
}

// DETERMINA A QUANTIDADE DE VERTICE NO GRAFO DE ARESTA =============================================================================================================

int quantidade_vertice_grafo_aresta(int ** matriz_adjacencia, FILE * arquivo){ // O(N²)
	int contador_quantidade_arestas = 0;
	
	// determina a quantidade de aresta que a matriz de entrada tem
	for(int i = 0; i < tamanho_matriz(arquivo); i++){ // O(N) * O(N) = O(N²)
		for(int j = 0; j < tamanho_matriz(arquivo); j++){ // O(N)
			if(matriz_adjacencia[i][j] == 1 && i != j){
				contador_quantidade_arestas++;
			} else
			if(matriz_adjacencia[i][j] == 1 && i == j){
				contador_quantidade_arestas = contador_quantidade_arestas + 2;
			}
		}
	}
	
	// quantidade de aresta divide por dois para ter  a quantidade de vertices presentes no grafo de aresta
	return (contador_quantidade_arestas/2);
}

// FORMA OS VERTICES DO GRAFO DE ARESTAS ================================================================================================================================

vertice * formar_vertices_grafo_aresta(int ** matriz_adjacencia, FILE * arquivo){ // O(N²)
	
	vertice * vertice_grafo_aresta;
	int indice_vertice_grafo_aresta = 0;
	int qnt_vertice = quantidade_vertice_grafo_aresta(matriz_adjacencia, arquivo); 	// quantidade de vertices do grafo de arestas // O(N²)
	int tamanho_mat =  tamanho_matriz(arquivo); 									// tamanho matriz de entrada // O(N)
	
	// aloca um vetor de registro para armazenar os vertices do grafo de aresta
	vertice_grafo_aresta = (vertice *) malloc(sizeof(vertice) * qnt_vertice);
	
	for(int i = 0; i < tamanho_mat; i++){ // O(N) * O(N) = O(N²)
		for(int j = 0; j < tamanho_mat; j++){
			// determina os pares de vertices que formam um vertice do grafo de aresta
			if(matriz_adjacencia[i][j] == 1 && (i < j || i == j)){ 
				vertice_grafo_aresta[indice_vertice_grafo_aresta].x = i + 1;
				vertice_grafo_aresta[indice_vertice_grafo_aresta].y = j + 1;
				indice_vertice_grafo_aresta++;
			}
		}
	}
	
	return vertice_grafo_aresta;
}

// LER A MATRIZ DE ADJACENCIA DO GRAFO DE ARESTAS ===========================================================================================================

void ler_grafo_aresta(vertice * vertice_grafo_aresta, int qnt_vertice_grafo_aresta){ // O(N²)
	
	// aloca uma matriz auxiliar 
	// atribui zero para toda a matriz_grafo_aresta 
	int ** matriz_grafo_aresta = (int **) malloc(sizeof(int *) * qnt_vertice_grafo_aresta);
	for(int c = 0; c < qnt_vertice_grafo_aresta; c++){ // O(N²)
		matriz_grafo_aresta[c] = (int *) malloc(sizeof(int) * qnt_vertice_grafo_aresta);
		for(int d = 0; d < qnt_vertice_grafo_aresta; d++){
			matriz_grafo_aresta[c][d] = 0;
		}
	}
	
	// organiza os vertices da matriz adjacencia do grafo de aresta -- (nomeia as colunas)
	for(int i = 0; i < qnt_vertice_grafo_aresta; i++){ // O(N²)
		printf("\t A(%d,%d)", vertice_grafo_aresta[i].x, vertice_grafo_aresta[i].y);
	}
	printf("\n");
	
	// determina as arestas do grafo de arestas atribuindo a matriz auxiliar
	for(int i = 0; i < qnt_vertice_grafo_aresta; i++){ 
		// nomeia as linhas 
		printf("A(%d,%d)", vertice_grafo_aresta[i].x, vertice_grafo_aresta[i].y);
		for(int j = 0; j < qnt_vertice_grafo_aresta; j++){ 
			// verifica se não é diagonal
			if(i != j){
				// verifica a partir de comparações para obter quais são as novas ligações, caso um vertice possua um vertice antigo em comum, conterá uma ligação no novo grafo
				if(vertice_grafo_aresta[i].x == vertice_grafo_aresta[j].x || vertice_grafo_aresta[i].x == vertice_grafo_aresta[j].y || vertice_grafo_aresta[i].y == vertice_grafo_aresta[j].x || vertice_grafo_aresta[i].y == vertice_grafo_aresta[j].y){
					matriz_grafo_aresta[i][j] = 1;
				} 
			}	
			printf("\t     %d", matriz_grafo_aresta[i][j]);
		}
		printf("\n");
	}
	
	// desaloca a matriz matriz_grafo_aresta (auxiliar)
	for(int c = 0; c < qnt_vertice_grafo_aresta; c++){ // O(N)
		free(matriz_grafo_aresta[c]);
	}
	free(matriz_grafo_aresta);
}

