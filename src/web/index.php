<?php
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
?>
<!DOCTYPE html>
<html lang="pt-br">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">

	<!-- bootstrap 5 -->
	<link href="vendor/bootstrap-5.2.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet">
	<script src="vendor/bootstrap-5.2.0-beta1/dist/js/bootstrap.bundle.min.js"></script>

	<!-- jquery 3.6.0 -->
	<script src="vendor/jquery/jquery-3.6.0.min.js"></script>

	<!-- FontAwesome Icons -->
	<link rel="stylesheet" href="vendor/font-awesome-4.7.0/css/font-awesome.min.css">

	<!-- SweetAlert2 -->
	<script src="vendor/sa2/sa2.js"></script>


	<title>Monitoramento</title>
</head>

<body class="bg-dark">

	<div class="p-5">
		<div class="list-group">
			<a class="list-group-item list-group-item-action active" aria-current="true">
				<div class="text-center">
					<div class="text-center">
						<h5 class="mb-1">Lista das 10 últimas medições</h5>
						<small class="row">
							<div class="col-md-6">
								<label class="badge bg-white text-dark ">
									Refresh: #</i><span id="count">0</span>
								</label>
							</div>
							<div class="col-md-6">
								<button type="button" class="badge bg-white text-dark btn btn-light" data-bs-toggle="modal" data-bs-target="#modaledit">
									<label class="">Intervalo: <span id="interval"></span>s
										<em class="fa fa-pencil" style="font-size: 12px; color: #000" aria-hidden="true"></em></label>
								</button>
							</div>
						</small>
						<div hidden id="alert-error" class="alert alert-danger mt-3" role="alert">
							<p id="alert-text-error" class="h5 m-0"></p>
						</div>
						<div hidden id="alert-info" class="alert alert-info mt-3" role="alert">
							<p id="alert-text-info" class="h5 m-0"></p>
						</div>
					</div>
				</div>
			</a>
			<div id="medicoes">
				<div id="loading">
					<a href="#" class="list-group-item list-group-item-action">
						<div class="d-flex w-100 justify-content-between">
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
						</div>
					</a>
				</div>
				<div class="lists">

				</div>
			</div>
		</div>
	</div>
	<!-- Modal -->
	<div class="modal fade" id="modaledit" tabindex="-1" aria-labelledby="modaleditLabel" aria-hidden="true">
		<div class="modal-dialog modal-dialog-centered">
			<div class="modal-content">
				<form id="form-interval">
					<div class="modal-header">
						<h5 class="modal-title" id="modaleditLabel">Editar intervalo de tempo</h5>
						<button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
					</div>
					<div hidden id="alert-error-modal" class="alert alert-danger m-0 text-center" role="alert">
						<p id="alert-text-error-modal" class="h5 m-0"></p>
					</div>
					<div class="modal-body p-lg-5">
						<div class="input-group input-group-lg">
							<input id="interval-input" name="interval" type="number" class="form-control text-center" aria-label="" aria-describedby="inputGroup-sizing-lg">
						</div>
					</div>
					<div class="modal-footer">
						<button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Cancelar</button>
						<button type="submit" class="btn btn-primary">Alterar</button>
					</div>
				</form>
			</div>
		</div>
	</div>
	<script>
		$(function(event) {

			$("#form-interval").submit((e) => {
				e.preventDefault();

				$('#alert-error-modal').attr('hidden', '');
				$('#alert-text-error-modal').text('');

				if ($("#interval-input").val() === "") {
					$('#alert-error-modal').removeAttr('hidden');
					$('#alert-text-error-modal').text("Por favor informe um intervalo.");
					return
				} else if ($("#interval-input").val() < 2) {
					$('#alert-error-modal').removeAttr('hidden');
					$('#alert-text-error-modal').text("O intervalo deve ser ao menos 2 segundos.");
					return
				}

				$.ajax({
					type: "POST",
					url: "controllers/interval.php",
					dataType: "json",
					data: $('#form-interval').serialize(),
					beforeSend: function() {
						$('#alert-error').attr('hidden', '');
						$('#alert-text-error').text('');
					},
					success: function(response) {
						if (response.success) {
							Swal.fire({
								icon: 'success',
								title: 'Sucesso!',
								text: 'Intervalo de tempo enviado: ' + response.data,
							});
							$("#modaledit").modal('hide');
						} else {
							Swal.fire({
								icon: 'error',
								title: 'Erro ao enviar intervalo!',
								text: response.error,
							});
						}

					},
					error: function(data) {
						console.log(data);
						Swal.fire({
							icon: 'error',
							title: 'Erro!',
							text: 'Parece que estamos offline. Chame o TI!',
						});
					}
				});
			});

			setInterval(requestData, 1000);

			async function requestData() {
				await $.ajax({
					type: "GET",
					url: "controllers/measures.php",
					dataType: "html",
					beforeSend: function() {
						$('#alert-error').attr('hidden', '');
						$('#alert-text-error').text('');
						$('#alert-info').attr('hidden', '');
						$('#alert-text-info').text('');

						if ($('#count').text() == '0') {
							$('#loading').removeAttr('hidden');
						}
					},
					success: function(data) {
						response = JSON.parse(data);
						if (response.success) {
							$('.lists').empty();

							if (response.data.measures == "") {
								$('#alert-info').removeAttr('hidden');
								$('#alert-text-info').text('Não há dados para carregar.');
								$("#interval").text('0');
							} else {
								$(".lists").append(response.data.measures);
								$("#interval").text(response.data.interval);
							}
							$('#count').text(parseInt($('#count').text()) + 1);
						} else {
							$('#alert-error').removeAttr('hidden');
							$('#alert-text-error').text('Não foi possível carregar a lista: ' + response.error);
						}

					},
					error: function(data) {
						console.log(data);
						$('#alert-error').removeAttr('hidden');
						$('#alert-text-error').text('Parece que estamos offline. Chame o TI!');
					},
					complete: function() {
						$('#priorities').removeAttr('hidden');
						$('#loading').attr('hidden', '');
					}
				});
			}
		});
	</script>

</body>

</html>