'use strict';

module.exports = function(grunt) {

  // Project configuration.
  grunt.initConfig({
    browserify: {
      lib: {
        src: ['lib/xbee-api.js'],
        dest: 'examples/chrome-tool/js/dist/xbee.js',
        options: {
          alias:'lib/xbee-api.js:xbee-api'
        }
      },
      src: {
        src: ['examples/chrome-tool/js/src/app.js'],
        dest: 'examples/chrome-tool/js/dist/main.js',
        options: {
          external: [ 'xbee-api' ]
        }
      }
    },
    nodeunit: {
      files: ['test/**/*_test.js'],
    },
    jshint: {
      options: {
        jshintrc: '.jshintrc'
      },
      gruntfile: {
        src: 'Gruntfile.js'
      },
      lib: {
        src: ['lib/**/*.js']
      },
      test: {
        src: ['test/**/*.js']
      },
    },
    watch: {
      gruntfile: {
        files: '<%= jshint.gruntfile.src %>',
        tasks: ['jshint:gruntfile']
      },
      lib: {
        files: '<%= jshint.lib.src %>',
        tasks: ['jshint:lib', 'nodeunit']
      },
      test: {
        files: '<%= jshint.test.src %>',
        tasks: ['jshint:test', 'nodeunit']
      },
    },
  });

  // These plugins provide necessary tasks.
  grunt.loadNpmTasks('grunt-contrib-nodeunit');
  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-browserify');

  // Default task.
  grunt.registerTask('chrome-example', ['browserify:lib', 'browserify:src' ]);
  grunt.registerTask('default', ['jshint', 'nodeunit', 'chrome-example']);

};
