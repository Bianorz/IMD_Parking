var http = require('http');
var fs = require('fs');
var path = require('path');

http.createServer(function (request, response) {
	var filePath = '.' + request.url;
	// se o endereço chamado for vazio chama example.html
	if (filePath == './')
		filePath = './vagas.html';

	// a função path.extname(filePath) detecta os tipos de extensão chamados de dentro do arquivo em 'filepath'
	var extname = path.extname(filePath);

	// contentType diz qual o tipo de conteudo que vai ser exibido, texto em html é o padrão
	var contentType = 'text/html';
	
	// modifica o tipo de cabeçalho de exibição baseado na extensão detectada por extname
	switch (extname) {
		case '.js':
			contentType = 'text/javascript';
		    break;
		case '.css':
			contentType = 'text/css';
		    break;
		case '.json':
			contentType = 'application/json';
		    break;
		case '.png':
			contentType = 'image/png';
		    break;      
		case '.jpg':
			contentType = 'image/jpg';
		    break;
		case '.wav':
			contentType = 'audio/wav';
		    break;
		case '.gif':
			contentType = 'image/gif';
		    break;
	}
	
	// o servidor finalmente inicia a exibição da pagina
	fs.readFile(filePath, function(error, content) {
		if (error) {
		// em caso de erro exibe o código do erro
			response.writeHead(500);
			response.end('Erro: '+error.code+' ..\n');
			response.end(); 
		}
		else {
		// exibe a página usando o cabeçalho apropriado ao tipo de conteúdo detectado por extname
			response.writeHead(200, { 'Content-Type': contentType });
			response.end(content, 'utf-8');
		}
	});

}).listen(8000);
