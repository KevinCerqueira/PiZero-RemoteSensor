/**
 * Componente Curricular: MI Concorrência e Conectividade
 * Autor: Esdras Abreu, Guilherme Nobre e Kevin Cerqueira
 *
 * Declaro que este código foi elaborado por mim de forma individual e
 * não contém nenhum trecho de código de outro colega ou de outro autor,
 * tais como provindos de livros e apostilas, e páginas ou documentos
 * eletrônicos da Internet. Qualquer trecho de código de outra autoria que
 * uma citação para o  não a minha está destacado com  autor e a fonte do
 * código, e estou ciente que estes trechos não serão considerados para fins
 * de avaliação. Alguns trechos do código podem coincidir com de outros
 * colegas pois estes foram discutidos em sessões tutorias.
 */

#include <stdio.h>
#include <mosquitto.h>

int main(){
	int user, connect;
	struct mosquitto * mosq;

	mosquitto_lib_init(); //Deve ser chamado antes de qualquer outra função mosquitto.

	mosq = mosquitto_new("G02_THEBESTGROUP_PUB_RASP", true, NULL);  //Crie uma nova instância do cliente mosquitto.
	// publisher = String a ser usada como ID do cliente. Se NULL, um ID de cliente aleatório será gerado. Se id for NULL, clean_session deve ser true.
	// true = definido como true para instruir o broker a limpar todas as mensagens e assinaturas na desconexão, false para instruí-lo a mantê-las.
	// NULL = Ponteiro para um inteiro que será usado como identificador de objeto.

	user = mosquitto_username_pw_set(mosq,"aluno","aluno*123"); //Configura o nome de usuário e a senha para uma instância do mosquitto.
	// aluno = Nome de usuário.
	// aluno*123 = Senha.
	connect = mosquitto_connect(mosq, "10.0.0.101", 1883, 5);

	// Retorna 0 se a configuração foi bem sucedida, ou um código de erro caso contrário.
	if(user != 0 || connect != 0){
		printf("Cliente não foi conectado ao Broken! Error Code: user %d\n e conexao %d", user, connect);
		mosquitto_destroy(mosq);
		return -1;
	}

	printf("Agora você está conectado ao Broken!\n");

	mosquitto_publish(mosq, NULL, "G02_THEBESTGROUP/MEDICOES", 50,"STRING DE MEDICOES", 0, false);
	// p1 uma instância de mosquito válida.
	// p2 ponteiro para um int. Se não for NULL, a função definirá isso para o ID da mensagem dessa mensagem específica. Isso pode ser usado com o retorno de chamada de publicação para determinar quando a mensagem foi enviada. Observe que, embora o protocolo MQTT não use IDs de mensagem para mensagens com QoS=0, libmosquitto atribui a eles IDs de mensagem para que possam ser rastreados com este parâmetro.
	// p3 Cadeia de caracteres terminada em nulo do tópico para publicação.
	// p4 o tamanho da carga útil (bytes). Os valores válidos estão entre 0 e 268.435.455.
	// p5 carga útil ponteiro para os dados a serem enviados. Se payloadlen > 0 este deve ser um local de memória válido.
	// p6 valor inteiro 0, 1 ou 2 indicando a Qualidade de Serviço a ser usada para a mensagem.
	// p7 reter	definido como true para manter a mensagem.

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();
	return 0;
}
