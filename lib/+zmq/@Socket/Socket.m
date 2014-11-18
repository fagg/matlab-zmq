classdef Socket < handle
    %Socket Summary of this class goes here
    %   Detailed explanation goes here

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
            socketType = strrep(upper(socketType), 'ZMQ_', '');
            socketType = strcat('ZMQ_', socketType);

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

end