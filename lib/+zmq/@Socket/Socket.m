classdef Socket < handle

    properties (Access = private)
        socketPointer;
    end

    properties (Access = public)
        bindings;
        connections;
        defaultBufferLength;
    end

    methods
        function obj = Socket(contextPointer, socketType)
            socketType = obj.normalize_const_name(socketType);
            % Core API
            obj.socketPointer = zmq.core.socket(contextPointer, socketType);
            % Init properties
            obj.bindings = {};
            obj.connections = {};
            obj.defaultBufferLength = 255;
        end

        function delete(obj)
            if (obj.socketPointer ~= 0)
                % Disconnect/Unbind all the endpoints
                cellfun(@(b) obj.unbind(b), obj.bindings, 'UniformOutput', false);
                cellfun(@(c) obj.disconnect(c), obj.connections, 'UniformOutput', false);
                % Avoid linger time
                obj.set('linger', 0);
                % close
                obj.close;
            end
        end
    end

    methods (Access = protected)
        normalized = normalize_const_name(obj, name);
        varargout = normalize_msg_options(obj, varargin);
    end

end
