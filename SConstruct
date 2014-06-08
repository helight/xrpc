import os
env = Environment()  # Initialize the environment

root_path = os.getcwd()
thirdparty = str(root_path) + "/thirdparty/"

env.Append(CPPPATH = [root_path, thirdparty, thirdparty + '/libev/include/'])
env.Append(LIBPATH = [thirdparty + '/libev/lib/', thirdparty + '/jsonrpc/', root_path + '/xsrv'])
env.Append(LIBS = ['jsonrpc', 'ev'])

env.Append(CCFLAGS = ['-Wall', '-O2', '-g'])
env.Append(LINKFLAGS = ['-static'])
Export('env')

SConscript(['xsrv/SConscript'], exports = 'env')
SConscript(['xsrv_demo/SConscript'], exports = 'env')
