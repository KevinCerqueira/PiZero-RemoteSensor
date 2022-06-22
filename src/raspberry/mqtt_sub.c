#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>

void on_connect(struct mosquitto *mosq, void *obj, int connect) {
	if(connect!=0){ 
		printf("Erro no resultado do codigo: %d\n", connect);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "G02_THEBESTGROUP/MEDICOES", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	printf("Nova mensagem com o tópico %s: %s\n", msg->topic, (char *) msg->payload);
}

int main() {
	int user, connect, id;
	id = 12;

	mosquitto_lib_init(); //Deve ser chamado antes de qualquer outra função mosquitto.
	struct mosquitto *mosq;

	mosq = mosquitto_new("G02_THEBESTGROUP_SUB_RASP", true, &id);//Cria uma nova instância do cliente mosquitto.
	// subscribe = String a ser usada como ID do cliente. Se NULL, um ID de cliente aleatório será gerado. Se id for NULL, clean_session deve ser true.
	// true = definido como true para instruir o broker a limpar todas as mensagens e assinaturas na desconexão, false para instruí-lo a mantê-las.
	// NULL = Ponteiro para um inteiro que será usado como identificador de objeto.

	mosquitto_connect_callback_set(mosq, on_connect);

	mosquitto_message_callback_set(mosq, on_message);

	user = mosquitto_username_pw_set(mosq,"aluno","aluno*123");
	connect = mosquitto_connect(mosq, "10.0.0.101", 1883, 5);

	if(user != 0 || connect != 0){
		printf("Cliente não foi conectado ao Broken! Error Code: user %d\n e conexao %d", user, connect);
		return -1;
	}

	mosquitto_loop_start(mosq);
	printf("Press Enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
