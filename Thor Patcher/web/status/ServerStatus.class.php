<?php


class ServerStatus {

# Declara��o das propriedades.
private $host;
private $user;
private $pass;
private $base;

	# M�todo construtor da classe.
	function __construct($host,$user,$pass,$base) {
		$this->host = gethostbyname( $host );
		$this->user = $user;
		$this->pass = $pass;
		$this->base = $base;
	}
	
	# M�todo destrutor da classe.
	function __destruct() {
		$this->host;
		$this->user;
		$this->pass;
		$this->base;
	}
	
	# M�todo para conectar com o host:porta.
	function connect( $host,$port ) {
		# $fp receber� o retorno da fun��o fsockopen.
		$fp = @fsockopen( $host, $port, $errno, $errstr, 1.0 );
		# $close receber� o retorno do m�todo $this->close;
		$close = $this->close( $fp );
		# Ir� retornar o valor de $fp.
		return $fp;
	}
	
	# M�todo para fechar a conex�o.
	function close( $fp ) {
		# Chama a fun��o fclose para fechar a conex�o gerada em $fp.
		@fclose( $fp );
	}
	
	# M�todo de retorno do Status atrav�s de $port.
	function getSt( $port ) {
		# Retorna o retorno do m�todo $this->connect.
		return $this->connect( $this->host, $port );
	}
	
	# M�todo de retorno do Statos do Login-Serv.
	function getLoginStatus() {
		# Retorna o retorno de $this->getSt com a porta 6900.
		return $this->getSt( 6900 );
	}
	
	# M�todo de retorno do Statos do Char-Serv.
	function getCharStatus() {
		# Retorna o retorno de $this->getSt com a porta 6121.
		return $this->getSt( 6121 );
	}
	
	# M�todo de retorno do Statos do Map-Serv.
	function getMapStatus() {
		# Retorna o retorno de $this->getSt com a porta 5121.
		return $this->getSt( 5121 );
	}
	
	# M�todo para pegar o n�mero de jogadores online.
	function getUsersOnline() {
		# $sql recebe o retorno da conex�o MySQL.
		$sql = mysql_connect( $this->host, $this->user, $this->pass ) or die( 'Sem conex�o com o servidor MySQL' );
		# $sel recebe o retorno da sele��o do Banco de Dados.
		$sel = mysql_select_db( $this->base ) or die( 'Banco de Dados n�o encontrado' );
		# $qry recebe o retorno da execu��o do comando SQL.
		$qry = mysql_query( "SELECT COUNT(online) as usersOnline FROM `char` WHERE `online` = 1" ) or die( 'Erro na tabela "char"' );
		# $res recebe o valor contido como array.
		$res = mysql_fetch_array( $qry );
		# Retorna o valor contido na array $res['usersOnline'].
		return $res['usersOnline'];
	}
}

?>