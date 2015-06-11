<html>
<head>
<title></title>
</head>
<link rel="stylesheet" href="style.css" type="text/css">
<body style="overflow-y: hidden">
	<tr>
		<td>
		<div>
		<table border="0" width="100%">
			<tr>
				<td><span class="header">Status do Servidor</span>
					<div class="content">
					
					<table border="0" width="100%" style="font-size: 95%;">
					<tr>
					<td>
						<?php

# Require no arquivo da classe.
require "ServerStatus.class.php";

# Instanciar a classe ServerStatus com os parâmetros: (IP, Usuário, Senha, Banco de Dados).
$status = new ServerStatus( "127.0.0.1", "root", "vertrigo", "ragnarok2" );

	# Se $status->getLoginStatus() retornar algum valor, imprime Online.
	if($status->getLoginStatus()) echo '<img src="images/login-on.gif" />';
	# Senão, imprime Offline
	else echo '<img src="images/login-off.gif" />';
	
	# Quebra de linha HTML.
	echo "<br /> <br />";
	
	# Se $status->getCharStatus() retornar algum valor, imprime Online.
	if($status->getCharStatus()) echo '<img src="images/char-on.gif" />';
	# Senão, imprime Offline.
	else echo '<img src="images/char-off.gif" />';
	
	# Quebra de linha HTML.
	echo "<br /> <br />";
	
	# Se $status->getMapStatus() retornar algum valor, imprime Online.
	if($status->getMapStatus()) echo '<img src="images/map-on.gif" />';
	# Senão, imprime Offline.
	else echo '<img src="images/map-off.gif" />';
	
	# Quebra de linha HTML.
	echo "<br /> <br />";
	

?>
					</td>
					</tr>
					
					</div>
					<table>
				</td>
				</tr>
		</table>
		</div>
		</td>
	</tr>
</body>
</html>
