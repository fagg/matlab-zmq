classdef Context < handle
   
    properties (Access = private)
        contextPointer = [];
    end
    
    methods
           function obj = Context(varargin)
               if (nargin ~= 0)
                   warning('zmq:Context:extraConstructArgs','Extraneous constructor arguments.');
               end
               obj.contextPointer = zmq.core.ctx_new();
           end
          
           function delete(obj)
              zmq.core.ctx_shutdown(obj.contextPointer);
              zmq.core.ctx_term(obj.contextPointer);
           end
    end
    
    methods (Access = protected)
        normalized = normalize_const_name(obj, name);
    end
end
