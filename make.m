% make.m
%
% Builds core mex API.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% ZMQ CONFIGURATION:
if ispc
  ZMQ_COMPILED_LIB = 'libzmq-v120-mt-4_0_4';
  ZMQ_INCLUDE_PATH = 'C:\Program Files\ZeroMQ 4.0.4\include\';
  ZMQ_LIB_PATH = 'C:\Program Files\ZeroMQ 4.0.4\lib\';
else
  ZMQ_COMPILED_LIB = 'zmq'
  ZMQ_INCLUDE_PATH = '/usr/local/Cellar/zeromq/4.0.4/include/';
  ZMQ_LIB_PATH = '/usr/local/Cellar/zeromq/4.0.4/lib/';
end


%% ZMQ GENERATED VARS

% --> Windows whitespace normalization :(
if ispc
  reduce_path = @(orig_path) system(['for %A in ("', orig_path ,'") do @echo %~sA']);
  [status, ZMQ_INCLUDE_PATH] = reduce_path(ZMQ_INCLUDE_PATH);
  ZMQ_INCLUDE_PATH = strtrim(ZMQ_INCLUDE_PATH);
  if status; error('mex:compile', 'Unable to recognize path'); end
  [status, ZMQ_LIB_PATH] = reduce_path(ZMQ_LIB_PATH);
  ZMQ_LIB_PATH = strtrim(ZMQ_LIB_PATH);
  if status; error('mex:compile', 'Unable to recognize path'); end
end
% <--

ZMQ_COMPILE_FLAGS = sprintf('-I"%s" -L"%s" -l"%s"', ZMQ_INCLUDE_PATH, ZMQ_LIB_PATH, ZMQ_COMPILED_LIB);

[MAKE_DIR, ~, ~] = fileparts(mfilename('fullpath'));
LIB_DIR = fullfile(MAKE_DIR, 'lib');
SRC_DIR = fullfile(MAKE_DIR, 'src', 'core');

%% CORE API FILE LIST
CORE_FILE_LIST = { ...
                fullfile(SRC_DIR, 'zmq_version.c'), ...
                fullfile(SRC_DIR, 'zmq_ctx_new.c'), ...
                fullfile(SRC_DIR, 'zmq_ctx_term.c'), ...
                fullfile(SRC_DIR, 'zmq_ctx_shutdown.c'), ...
                fullfile(SRC_DIR, 'zmq_ctx_get.c'), ...
                fullfile(SRC_DIR, 'zmq_ctx_set.c'), ...
                fullfile(SRC_DIR, 'zmq_socket.c') ...
                };


%% BUILD
BUILD_FUNCTION = @(file) eval(['mex -largeArrayDims -O ', ZMQ_COMPILE_FLAGS, sprintf(' "%s" -outdir %s', char(file), LIB_DIR)]);
cellfun(BUILD_FUNCTION, CORE_FILE_LIST);