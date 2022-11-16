from os.path import join

def module_init(env, parameter=''):
    return
    name = "usb"
    print( "  *", name.upper(), "CDC" )

    p = join( env.framework_dir, "sdk", "middleware", "MTK", name) 
    p = join( env.framework_dir, "wizio", name) 
    env.Append( 
        CPPDEFINES = [ "MTK_USB_DEMO_ENABLED", "MTK_USB11", "MTK_USB2UART_DMA_THRU" ],
        CPPPATH = [ join( p, "inc" ) ] 
    )   
    filter = ["-<*>",
        "+<%s>" % join( p, "src", "cdc" ),
        "+<%s>" % join( p, "src", "common" ),
    ] 
    env.BuildSources( join( "$BUILD_DIR", "modules", "usb" ), p, filter )

