var File = function(path, size/*, watcher*/){
    this.path = function(){
        return path;
    };

    /* this.watch = function(cbk){
        window.Dubo.nodules.watcher.watch(path, cbk);
    };

    this.unwatch = function(cbk){
        window.Dubo.nodules.watcher.forget(path, cbk);
    };*/
};

var FileSystem = function(root, onscanned){
    var dirty = 1;

    var rootDir;

    this.getRoot = function(){
        return rootDir;
    };

    this.isDirty = function(){
        return !!dirty;
    };

    // And a walker
    Dubo.nodules.walker.newWalker(function(walker){

        // Connect slots to be notified of files and dirs, for the initial population run
        walker.hasFile.connect(function(f, s){
            var sub = f.substr(root.length).split("/");
            sub.shift();
            var pth = root;
            var key;
            var dir = rootDir;
            while(sub.length > 1){
                key = sub.shift();
                // pth = pth + "/" + key;
                // pth = pth + "/" + key;
                // dir = (dir.getDirs())[pth];
                dir = (dir.getDirs())[key];
            }
            // var fkey = pth + "/" + sub.shift();
            var fkey = sub.shift();
            var lfiles = dir.getFiles();
            lfiles[fkey] = new File(fkey, s);
        });

        walker.hasDir.connect(function(f){
            var sub = f.substr(root.length).split("/");
            sub.shift();
            var pth = root;
            var key;
            var dir = rootDir;
            while(sub.length > 1){
                key = sub.shift();
//                    pth = pth + "/" + key;
//                    dir = (dir.getDirs())[pth];
                dir = (dir.getDirs())[key];
            }
            // var fkey = pth + "/" + sub.shift();
            var fkey = sub.shift();
            var lfiles = dir.getDirs();
            lfiles[fkey] = new Dir(f, walker, Dubo.nodules.watcher);
        });

        walker.done.connect(function(){
            dirty = 0;
            rootDir.initialized();
            onscanned();
        });

        rootDir = new Dir(root, walker, Dubo.nodules.watcher);

        // Deep scan
        walker.scan(root, true);
    });
};

var Dir = function(root, parentWalker, parentWatcher){
    // Start dirty, until the full scan completes
    var dirty = 1;

    var dirs = {};
    var files = {};

    var newDirsTree = {};
    var newFilesTree = {};

    this.path = function(){
        return root;
    };

    this.isDirty = function(){
        return !!dirty;
    };

    this.initialized = function(){
        dirty--;
        if (dirty > 0){
            // Whatever the dirty count, we just need to rescan once if we are here
            dirty = 1;
            walker.scan(root, false);
        }
        Object.keys(dirs).forEach(function(key){
            dirs[key].initialized();
        });
    };

    this.getFiles = function(){
        return files;
    };

    this.getDirs = function(){
        return dirs;
    };

    // Fork out a watcher
    parentWatcher.newWatcher(function(watcher){
        // And a walker
        parentWalker.newWalker(function(walker){
            // Connect the watcher
            watcher.directoryChanged.connect(function(f){
                // XXX ideally, we should cancel already ongoing refreshes
                dirty++;
                console.warn("Directory changed. Incrementing dirty count to " + dirty);
                // If this is freshly dirty and no scan was in progress, kick it going
                if (dirty === 1)
                    walker.scan(root, false);
            });

            // Connect slots to be notified of files and dirs
            walker.hasFile.connect(function(f, s){
                f = f.split("/").pop();
                if (f in files){
                    // Reuse existing objects
                    newFilesTree[f] = files[f];
                    delete files[f];
                }else{
                    newFilesTree[f] = new File(f, s);
                }
            });

            walker.hasDir.connect(function(f){
                f = f.split("/").pop();
                if (f in dirs){
                    // Reuse existing objects
                    newDirsTree[f] = dirs[f];
                    delete dirs[f];
                }else{
                    newDirsTree[f] = new Dir(f, walker, watcher);
                }
            });

            walker.done.connect(function(){
                console.warn("Completed the refresh");
                // XXX compare trees and notify for whatever shit has changed
                // XXX delete remaining objects and remove watchers
                // Copy over new data
                files = newFilesTree;
                dirs = newDirsTree;
                newFilesTree = {};
                newDirsTree = {};
                dirty--;
                if (dirty > 0){
                    // Whatever the dirty count, we just need to rescan once if we are here
                    dirty = 1;
                    walker.scan(root, false);
                }
            });

            // Watch it
            watcher.watch(root);
        });
    });
};



const imageURI = "https://avatars1.githubusercontent.com/u/19539395?s=100&amp;v=4";

// XXX toggable menu item are changing value AFTER being triggered
var menuDemo = function(e){
    e.API.menu.addMenu(function(menu){
        menu.title = "Main menu";

        menu.addAction(menu.NOROLE, function(action){
            console.warn("Action icon is" + action.icon);
            action.title = "NOROLE∞";
            action.icon.URI = imageURI;
            action.checkable = true;
            action.checked = true;
            action.shortcut = "CTRL+P";
            action.autoRepeat = true;
            action.updated.connect(function(data){
                console.warn("UPDATED" + action.checked);
            });
            action.triggered.connect(function(data){
                console.warn("TRIGGERED" + action.checked);
            });
        });
        menu.addAction(menu.TEXT, function(action){
            console.warn("Action is" + action);
            action.title = "TEXT∞";
            action.icon.URI = imageURI;
            action.checkable = true;
            action.checked = true;
            action.shortcut = "CTRL+O";
            action.autoRepeat = false;
            console.warn("action.autoRepeat" + action.autoRepeat);
        });
        menu.addAction(menu.APPLICATION, function(action){
            console.warn("Action is" + action);
            action.title = "APPLICATION∞";
            action.icon.URI = imageURI;
            action.checkable = true;
            action.checked = true;
        });
        menu.addAction(menu.ABOUTQT, function(action){
            console.warn("Action is" + action);
            action.title = "ABOUTQT∞";
            action.icon.URI = imageURI;
            action.checkable = true;
            action.checked = true;
        });
        menu.addAction(menu.ABOUT, function(action){
            console.warn("Action is" + action);
            action.title = "ABOUT∞";
            action.icon.URI = imageURI;
            action.checkable = true;
            action.checked = true;
        });
        menu.addAction(menu.PREFERENCES, function(action){
            console.warn("Action is" + action);
            action.title = "PREFERENCES∞";
            action.icon.URI = imageURI;
            action.checkable = true;
            action.checked = true;
        });
        menu.addAction(menu.QUIT, function(action){
            console.warn("Action is" + action);
            action.title = "QUIT∞";
            action.icon.URI = imageURI;
            action.checkable = true;
            action.checked = true;
        });

        menu.addMenu(function(sub){
            sub.title = "Sub Menu";
            sub.icon.URI = imageURI;
            sub.checkable = true;
            sub.checked = true;
        });
    });
};

var trayDemo = function(e){
    e.API.tray.menu.addMenu(function(menu){
        menu.title = "Submenu";
    });
    e.API.tray.menu.addAction(e.API.tray.menu.NOROLE, function(action){
        action.title = "Action";
    });
    e.API.tray.icon.URI = imageURI;
    e.API.tray.tooltip = "Foo foo";
    e.API.tray.visible = true;

    e.API.tray.activated.connect(function(reason){
        console.warn("Been activated with reason" + reason);
    });
};

window.addEventListener("roxee", function(e){
    menuDemo(e);
    trayDemo(e);

    // Network demo
    var n = e.API.network;
    console.warn(">" + n.diskCache.maximumSize);
    console.warn(">" + n.diskCache.currentSize);
    console.warn(">" + n.accessible);

    n.networkChanged.connect(function(result){
        console.warn("Network changed " + result);
    });

    n.cookieJar.updated.connect(function(){
        console.warn("Cookies updated: " + JSON.stringify(n.cookieJar.cookies, 2));
    });

    n.request("https://www.google.com", "GET", {"accept": "foo/bar"}, "", function(reply){
        reply.done.connect(function(err, status, headers, data){
            console.warn("Response error: " + err);
            console.warn("Response status: " + status);
            console.warn("Response headers: " + JSON.stringify(headers));
        });
    });

    var readRequest = function(sock, cbk){
        var req = "";
        var read = function(){
            sock.read(function(line){
                req += line;
                if (line)
                    return read();
                cbk(req);
            });
        };
        read();
    };

    // Very rudimentary network server
    e.API.server.start("8080", function(){
        console.warn("Server started");
        e.API.server.incoming.connect(function(sock){
            console.warn("GOT stuff on the socket");
            readRequest(sock, function(resp){
                console.warn("GOT " + resp);
            });
        });

        n.request("http://localhost:8080/", "GET", {"User-Agent": "foo/bar/baz"}, "foulalala ∞", function(reply){
            reply.done.connect(function(err, status, headers, data){
                console.warn("Response error: " + err);
                console.warn("Response status: " + status);
                console.warn("Response headers: " + JSON.stringify(headers));
            });
        });
    });


    // Display library information
    var infos = e.API.root;
    var iNode = document.getElementById("info");
    var list = ["NAME", "VENDOR", "VERSION", "REVISION", "CHANGESET", "BUILD", "LINK", "QT", "PLUGIN_NAME", "PLUGIN_VERSION", "PLUGIN_REVISION"];

    list.forEach(function(i){
        var line = document.createElement("div");
        line.appendChild(document.createTextNode(i + ": " + infos[i]));
        iNode.appendChild(line)
    });

    var hook = function(id, func){
        var n = document.getElementById(id);
        n.removeAttribute("disabled");
        n.onclick = func;
    };

    // FileSystem: open
    hook("open", function(){
        e.API.helpers.open("/Users/dmp/.profile");
    });

    // FileSystem: pick dir
    hook("pick", function(){
        e.API.helpers.pick("/Users/dmp", "Foo foo la la", function(choice){
            var n = document.getElementById("pick");
            n.appendChild(document.createTextNode("Previously picked:" + choice));
        });
    });

    // FileSystem: dir exists
    e.API.helpers.dirExists("/Users/dmp", function(res){
        var n = document.getElementById("direxists");
        n.appendChild(document.createTextNode("Dir exists?" + res));
    });

    // FileSystem: file exists
    e.API.helpers.fileExists("/Users/dmp/.profile", function(res){
        var n = document.getElementById("fileexists");
        n.appendChild(document.createTextNode("File exists?" + res));
    });

    // FileSystem: file size
    e.API.helpers.fileSize("/Users/dmp/.profile", function(res){
        var n = document.getElementById("filesize");
        n.appendChild(document.createTextNode("File size?" + res));
    });

    // FileSystem: file hash
    e.API.helpers.fileHash("/Users/dmp/.profile", function(res){
        var n = document.getElementById("filehash");
        n.appendChild(document.createTextNode("File hash?" + res));
    });

    var debug = function(o){
        var d = o.getFiles();
        Object.keys(d).forEach(function(f){
            console.warn("File: " + d[f].path());
        });
        d = o.getDirs();
        Object.keys(d).forEach(function(f){
            console.warn("Dir: " + d[f].path());
            debug(d[f]);
        });
    };

    // FileSystem: scan & watch
    var fs = new FileSystem("/Users/dmp/tmp/log", function(){
        console.warn("Scan complete for: " + fs.getRoot().path());
        debug(fs.getRoot());
    });


    /*
    hook("mkdir", function(){
        e.API.helpers.makeDir("/Users/dmp/tmp/foolal", function(result){
            console.warn("Success?" + result);
        });
    });
*/
    /*
    <button disabled="disabled" id="mkdir"></button>
    <button disabled="disabled" id="rmdir"></button>

    Q_INVOKABLE static bool removeDir(const QString &dirName);
    Q_INVOKABLE static bool makeDir(const QString &dirName);
    */
});

window.addEventListener("load", function(){
    return new QWebChannel(qt.webChannelTransport, function(channel){
        var e = new Event("roxee");
        window.Dubo = window.Dubo || {};
        e.API = window.Dubo["nodules"] = {
            root: channel.objects.Root,
            helpers: channel.objects.Helpers,
            watcher: channel.objects.Watcher,
            walker: channel.objects.Walker,
            network: channel.objects.Network,
            server: channel.objects.NetServe,
            menu: channel.objects.MenuBar,
            tray: channel.objects.Tray,
        };
        window.dispatchEvent(e);
    });
});

