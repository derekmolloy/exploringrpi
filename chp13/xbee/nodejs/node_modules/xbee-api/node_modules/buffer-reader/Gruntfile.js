module.exports = function(grunt) {
    grunt.initConfig({
        pkg: grunt.file.readJSON("package.json"),
        jshint: {
            all: ['Gruntfilejs', '*.js'],
            options: {
                jshintrc: '.jshintrc'
            }
        },
        mochaTest: {
          test: {
            options: {
              reporter: 'spec'
            },
            src: ['test.js']
          }
        },
        watch: {
            options: {
                nospawn: false
            },
            js: {
                files:['*.js'],
                tasks: ['jshint', 'test']
            }
        }
    });


    grunt.loadNpmTasks("grunt-contrib-jshint");
    grunt.loadNpmTasks("grunt-contrib-watch");
    grunt.loadNpmTasks("grunt-mocha-test");
    
    grunt.registerTask('test', ['mochaTest']);
    grunt.registerTask("default", ["jshint", 'test']);
};

