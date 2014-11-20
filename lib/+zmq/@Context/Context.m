classdef Context < handle

    properties (Access = private)
        contextPointer;
        spawnedSockets;
    end

    methods
        function obj = Context(varargin)
            if (nargin ~= 0)
                warning('zmq:Context:extraConstructArgs','Extraneous constructor arguments.');
            end
            % Core API
            obj.contextPointer = zmq.core.ctx_new();
            % Initi properties
            obj.spawnedSockets = {};
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
