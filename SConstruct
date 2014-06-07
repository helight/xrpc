import os
env = Environment()  # Initialize the environment

root_path = "/data/helight_project/xsrver3"
#os.getcwd()
print root_path
thirdparty = str(root_path) + "/thirdparty/"
print thirdparty
env.Append(LIBPATH = [thirdparty + '/libev/lib/', thirdparty + '/jsonrpc/'])
env.Append(CPPPATH = [thirdparty + '/libev/include/', thirdparty + '/jsonrpc/'])
env.Append(LIBS = ['ev'])
env.Append(LINKFLAGS = ['-static'])
Export('env')

SConscript(['test/SConscript'], exports = 'env')
