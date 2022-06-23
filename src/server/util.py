"""
 * Componente Curricular: MI Concorrência e Conectividade
 * Autor: 
 *
 * Declaro que este código foi elaborado por mim de forma individual e
 * não contém nenhum trecho de código de outro colega ou de outro autor,
 * tais como provindos de livros e apostilas, e páginas ou documentos
 * eletrônicos da Internet. Qualquer trecho de código de outra autoria que
 * uma citação para o  não a minha está destacado com  autor e a fonte do
 * código, e estou ciente que estes trechos não serão considerados para fins
 * de avaliação. Alguns trechos do código podem coincidir com de outros
 * colegas pois estes foram discutidos em sessões tutorias.
"""
import os
import platform
from datetime import datetime
from datetime import date

# Pega os dados no arquivo .env (variáveis de ambiente)
def env(var):
	env = '\\..\\.env'
	if(platform.system() == 'Linux'):
		env = '/../.env'
		
	with open(os.path.dirname(os.path.realpath(__file__)) + env, 'r', encoding='utf-8') as file_env:
		line = file_env.readline()
		while(line):
			content = line.split('=')
			if(content[0] == var):
				return content[1]
			line = file_env.readline()

# Grava mensagens no LOG do sistema (arquivo logs/log_ANO-MES-DIA.log)
def log(origin, msg):
	with open(os.path.dirname(os.path.realpath(__file__)) + '/logs/log_' + str(date.today()) + '.log', 'a', encoding='utf-8') as log_file:
		log_file.write("[" + str(datetime.now()) + "][" + origin + "] " + msg + '\n')
	return True