"""
 * Componente Curricular: 
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
import sys
import json

# Classe para controlar a base de dados.
class DatabaseControl:
	# Base de dados de login dos medicos	
	db_user_path = '\\database\\doctors.json'
	# Base de dados dos pacientes
	db_patient_path = '\\database\\patients\\'
	
	# Diretorio do programa
	directory = ''
	
	# Construtor
	def __init__(self):
		self.directory = os.path.dirname(os.path.realpath(__file__))
	
	# Cria login do medico
	def createDoctor(self, username, auth):
		# Checando se já existe um usuario com o username igual
		if(self.checkUsername(username)):
			return False
		
		# Novos dados para adicionar a base
		id_doctor = int(self.getLastId(self.db_user_path)) + 1
		new_doctor = {'id': id_doctor, 'username': username, 'auth': auth}
		
		# Criando pasta e arquivo de pacientes do médico
		self.checkDoctorPath(username)
		with open(self.directory + self.db_patient_path + username + '\\patients.json', 'w', encoding='utf-8') as file:
			json.dump({}, file, ensure_ascii=False, indent=2)
		
		# Atualizando o arquivo dos medicos
		current_data = self.getAll(self.db_user_path)
		current_data[str(id_doctor)] = new_doctor
		with open(self.directory + self.db_user_path, 'w', encoding='utf-8') as db_write:
			json.dump(current_data, db_write, ensure_ascii=False, indent=4)
		return True
	
	# Cadastra novo paciente
	def createPatient(self, doctor_username, data):
		
		# Verifica se existe a pasta do medico
		self.checkDoctorPath(doctor_username)
		
		# Base de dados do paciente do medico em questao
		path = self.db_patient_path + doctor_username + '\\patients.json'
		
		# Atribuindo um novo ID para o paciente
		new_id = int(self.getLastId(path)) + 1
		new_data = {'id': new_id, 'nome': data['nome'], 'idade': data['idade'], 'sexo': data['sexo'], 'medicao': False, 'saturacao': 0, 'pressao': 0, 'batimento': 0, 'temperatura': 0.0}
		
		# Pega todos os pacientes daquele medico e adiciona o novo no final deles
		current_data = self.getAll(path)
		current_data[str(new_id)] = new_data
		with open(self.directory + path, 'w', encoding='utf-8') as db_write:
			json.dump(current_data, db_write, ensure_ascii=False, indent=4)
		return new_id
	
	# Retornar o último ID cadastrado para determinado arquivo.
	def getLastId(self, db_file):
		last_id = -1 # Retorna -1 caso não tenha nada.
		with open(self.directory + db_file, 'r', encoding='utf-8') as db_read:
			data = json.load(db_read)
			for id in data:
				last_id = id
		return last_id
	
	# Retornar todos os dados de um determinado arquivo.
	def getAll(self, db_file):
		with open(self.directory + db_file, 'r', encoding='utf-8') as db_read:
			return json.load(db_read)
		return None
	
	# Retornar um dado caso exista de um determinado arquivo, caso não retorna vazio.
	def get(self, db_file, id):
		with open(self.directory + db_file, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for index in data_read:
				if(str(index) == str(id)):
					return data_read[str(id)]
		return None
	
	# Verifica se o nome passado por parametro já existe na base de dados
	def checkUsername(self, username):
		with open(self.directory + self.db_user_path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for id_read in data_read:
				if(data_read[id_read]['username'] == username):
					return True
		return False
	
	# Cria/Checa se o medico tem uma pasta criada
	def checkDoctorPath(self, username):
		try:
			os.mkdir(self.directory + self.db_patient_path + username)
		except:
			pass
	
	# Retorna o token do medico com base no login
	def getTokenByLogin(self, username, auth):
		with open(self.directory + self.db_user_path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for id_read in data_read:
				if(data_read[id_read]['username'] == username and data_read[id_read]['auth'] == auth):
					return data_read[id_read]['auth']
		return None
	
	# Retorna os dados do medico com base no token
	def getDoctorByToken(self, token):
		with open(self.directory + self.db_user_path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for id_read in data_read:
				if(data_read[id_read]['auth'] == token):
					return data_read[id_read]
		return None
	
	# Verifica se o token é valido, isto é, se existe na base de dados
	def checkToken(self, token):
		with open(self.directory + self.db_user_path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for id_read in data_read:
				if(data_read[id_read]['auth'] == token):
					return True
		return False
	
	# Retornar um medico com base no ID.
	def getDoctorById(self, id):
		with open(self.directory + self.db_user_path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for id_read in data_read:
				if(str(id_read) == str(id)):
					return data_read[id_read]
		return None
	
	# Retorna um medico com base no seu nome de usuario
	def getDoctorByUsername(self, username):
		with open(self.directory + self.db_user_path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for id_read in data_read:
				if(data_read[id_read]['username'] == username):
					return data_read[id_read]
		return False
	
	# Retorna todos os pacientes de um determinado medico
	def getPatientsByDoctor(self, doctor_username):
		return self.getAll(self.db_patient_path + doctor_username + '\\patients.json')
	
	# Retorna um paciente de um determinado medico
	def getPatientByDoctor(self, doctor_username, id_patient):
		return self.get(self.db_patient_path + doctor_username + '\\patients.json', id_patient)
	
	# Atualiza um atributo em especifico de determinado paciente.
	def updateAttribute(self, doctor_username, patient_id, attr, value):
		
		# Verifica se o paciente realmente existe na base de dados do medico
		if(not self.has(doctor_username, patient_id)):
			return False
		
		path = self.directory + self.db_patient_path + doctor_username + '\\patients.json'
		with open(path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			
			data_read[str(patient_id)]['medicao'] = True
			data_read[str(patient_id)][attr] = value
			with open(path, 'w', encoding='utf-8') as db_write:
				json.dump(data_read, db_write, ensure_ascii=False, indent=4)
			return True
	
	# Atualiza os atributos de um paciente por completo.
	def updatePatient(self, doctor_username, patient_id, attrs):
		if(not self.has(doctor_username, patient_id)):
			return False
		path = self.directory + self.db_patient_path + doctor_username + '\\patients.json'
		with open(path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			
			data_read[str(patient_id)]['medicao'] = True
			data_read[str(patient_id)]['saturacao'] = attrs['saturacao']
			data_read[str(patient_id)]['pressao'] = attrs['pressao']
			data_read[str(patient_id)]['batimento'] = attrs['batimento']
			data_read[str(patient_id)]['temperatura'] = attrs['temperatura']
			
			with open(path, 'w', encoding='utf-8') as db_write:
				json.dump(data_read, db_write, ensure_ascii=False, indent=4)
			return True
	
	# Verifica se existe um paciente com base no nome de usuario do medico.
	def has(self, doctor_username, id):
		with open(self.directory + self.db_patient_path + doctor_username + '\\patients.json', 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			for id_read in data_read:
				if(str(id_read) == str(id)):
					return True
		return False
	
	# Deleta um paciente por ID.
	def deletePatient(self, doctor_username, patient_id):
		if(not self.has(doctor_username, patient_id)):
			return False
		path = self.directory + self.db_patient_path + doctor_username + '\\patients.json'
		with open(path, 'r', encoding='utf-8') as db_read:
			data_read = json.load(db_read)
			data_read.pop(str(patient_id))
			with open(path, 'w', encoding='utf-8') as db_write:
				json.dump(data_read, db_write, ensure_ascii=False, indent=4)
			return True
		return False