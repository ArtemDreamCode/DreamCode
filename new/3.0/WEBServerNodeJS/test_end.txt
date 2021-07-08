const http = require('http');
const url = require('url');

http.createServer((request, response) => {
    console.log('server work');
    if (request.method == 'GET') {
        // GET -> получить обработать
        let urlRequest = url.parse(request.url, true);
        console.log(urlRequest.query.test); // ! GET Params
        if (urlRequest.query.test % 2 == 0) {
            response.end('even');
        }
        response.end('odd');
    }


}).listen(3000);