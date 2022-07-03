"""
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
"""
import os
import platform
from datetime import datetime
from datetime import date

# Arquivo para gravar o log nos arquivos

# Verificação se a plataforma é Linux ou outro
path_log = '/logs/log_'
if(platform.system() != 'Linux'):
	path_log = '\\logs\\log_'

# Grava mensagens no LOG do sistema (arquivo logs/log_ANO-MES-DIA.log)
def log(origin, msg):
	with open(os.path.dirname(os.path.realpath(__file__)) + path_log + str(date.today()) + '.log', 'a', encoding='utf-8') as log_file:
		log_file.write("[" + str(datetime.now()) + "][" + origin + "] " + msg + '\n')
	return True