from os.path import join

def module_init(env, parameter=''):
    name = "system - minimal"
    print( "  *", name.upper() )

    env.Append( 
        LINKFLAGS = [                    
            "-Wl,-wrap=malloc",
            "-Wl,-wrap=calloc",
            "-Wl,-wrap=realloc",
            "-Wl,-wrap=free",
        ]
    )  

    p = join( env.framework_dir, "sdk", "kernel", "service")
    env.Append( 
        CPPPATH = [ join( p, "inc" ) ] 
    )   
    filter = ["-<*>",
        "+<%s>" % join( p, "src", "exception_handler.c" ),
        "+<%s>" % join( p, "src", "syslog.c" ),
        "+<%s>" % join( p, "src", "os_port_callback.c" ),
        "+<%s>" % join( p, "src", "memory_regions.c" ),
    ] 
    env.BuildSources( 
        join( "$BUILD_DIR", "modules", "service" ), 
        p, filter
    )

    p = join( env.framework_dir, "sdk", "driver", "board", "mt2625_hdk", "ept")
    env.Append( 
        CPPPATH = [ join( p, "inc" ) ] 
    )   
    env.BuildSources( 
        join( "$BUILD_DIR", "modules", "board" ), 
        join( p, "src")
    )
