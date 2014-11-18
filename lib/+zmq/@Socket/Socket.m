classdef Socket < handle
    
    properties (Access = private)
        socketPointer;
    end

    properties (Access = public)
        bindings = {};
        connections = {};
    end

    methods
        function obj = Socket(contextPointer, socketType)
            % Normalize 'type' parameter:
            socketType = obj.normalize_const_name(socketType);
            obj.socketPointer = zmq.core.socket(contextPointer, socketType);
        end

        function delete(obj)
            cellfun(@(b) obj.unbind(b), obj.bindings, 'UniformOutput', false);
            cellfun(@(c) obj.disconnect(c), obj.connections, 'UniformOutput', false);
            if (obj.socketPointer ~= 0)
                obj.close;
            end
        end
    end

    methods (Access = protected)
        normalized = normalize_const_name(obj, name);
        varargout = normalize_msg_options(obj, varargin);
    end

end
