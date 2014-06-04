/**
 * CrawlController
 *
 * @module      :: Controller
 * @description	:: A set of functions called `actions`.
 *
 *                 Actions contain code telling Sails how to respond to a certain type of request.
 *                 (i.e. do stuff, then send some JSON, show an HTML page, or redirect to another URL)
 *
 *                 You can configure the blueprint URLs which trigger these actions (`config/controllers.js`)
 *                 and/or override them with custom routes (`config/routes.js`)
 *
 *                 NOTE: The code you write here supports both HTTP and Socket.io automatically.
 *
 * @docs        :: http://sailsjs.org/#!documentation/controllers
 */

// On va utiliser la lib net 

var net = require('net');

module.exports = {

    index: function(req, res) {
        if (req.user) res.view({
            user: req.user
        });
        else res.view('home/index', {
            'error': '',
            'user': req.user });

    },

    crawl: function(req, res) {
        var url = req.param('url');

        var client = net.connect({
            port: 3002,
            host: '192.168.1.24'
        }, function () {
            client.write(url);

            // On delete le cache pour remettre à jour les recherches
            Search.destroy({ id: { '>': 0 }}).exec(function(err){});
        });

        res.view('home/index', {
            'error': '',
            'user': req.user });

    }

};
