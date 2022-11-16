from os.path import join
import subprocess
import shutil

def module_init(env, parameter=''):
    rc = subprocess.call( 
        [ 
            "C:/MinGW/bin/make",
            "-B", "all", 
            "-C", join( env.subst( "$PROJECT_DIR" ), "openapi-lib"),  
            '-s' 
        ],
    )
    if 0== rc:
        shutil.copy(
            join( env.subst( "$PROJECT_DIR" ), "openapi-lib", "libopenapi.a"), 
            "C:/Users/1124/.platformio/packages/framework-wizio-MT2625/pg/lib"
        )
        shutil.copy(
            join( env.subst( "$PROJECT_DIR" ), "openapi-lib", "libopenapi.a"), 
            "C:/Users/1124/.platformio/packages/framework-wizio-MT2625/openapi/lib"
        )        