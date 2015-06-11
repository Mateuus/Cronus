<?php


class ServerStatus {

# Declaraзгo das propriedades.
private $host;
private $user;
private $pass;
private $base;

	# Mйtodo construtor da classe.
	function __construct($host,$user,$pass,$base) {
		$this->host = gethostbyname( $host );
		$this->user = $user;
		$this->pass = $pass;
		$this->base = $base;
	}
	
	# Mйtodo destrutor da classe.
	function __destruct() {
		$this->host;
		$this->user;
		$this->pass;
		$this->base;
	}
	
	# Mйtodo para conectar com o host:porta.
	function connect( $host,$port ) {
		# $fp receberб o retorno da funзгo fsockopen.
		$fp = @fsockopen( $host, $port, $errno, $errstr, 1.0 );
		# $close receberб o retorno do mйtodo $this->close;
		$close = $this->close( $fp );
		# Irб retornar o valor de $fp.
		return $fp;
	}
	
	# Mйtodo para fechar a conexгo.
	function close( $fp ) {
		# Chama a funзгo fclose para fechar a conexгo gerada em $fp.
		@fclose( $fp );
	}
	
	# Mйtodo de retorno do Status atravйs de $port.
	function getSt( $port ) {
		# Retorna o retorno do mйtodo $this->connect.
		return $this->connect( $this->host, $port );
	}
	
	# Mйtodo de retorno do Statos do Login-Serv.
	function getLoginStatus() {
		# Retorna o retorno de $this->getSt com a porta 6900.
		return $this->getSt( 6900 );
	}
	
	# Mйtodo de retorno do Statos do Char-Serv.
	function getCharStatus() {
		# Retorna o retorno de $this->getSt com a porta 6121.
		return $this->getSt( 6121 );
	}
	
	# Mйtodo de retorno do Statos do Map-Serv.
	function getMapStatus() {
		# Retorna o retorno de $this->getSt com a porta 5121.
		return $this->getSt( 5121 );
	}
	
	# Mйtodo para pegar o nъmero de jogadores online.
	function getUsersOnline() {
		# $sql recebe o retorno da conexгo MySQL.
		$sql = mysql_connect( $this->host, $this->user, $this->pass ) or die( 'Sem conexгo com o servidor MySQL' );
		# $sel recebe o retorno da seleзгo do Banco de Dados.
		$sel = mysql_select_db( $this->base ) or die( 'Banco de Dados nгo encontrado' );
		# $qry recebe o retorno da execuзгo do comando SQL.
		$qry = mysql_query( "SELECT COUNT(online) as usersOnline FROM `char` WHERE `online` = 1" ) or die( 'Erro na tabela "char"' );
		# $res recebe o valor contido como array.
		$res = mysql_fetch_array( $qry );
		# Retorna o valor contido na array $res['usersOnline'].
		return $res['usersOnline'];
	}
}

?>