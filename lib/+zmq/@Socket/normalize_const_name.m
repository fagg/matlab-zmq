function normalized = normalize_const_name(~, name)
    normalized = strrep(upper(name), 'ZMQ_', '');
    normalized = strcat('ZMQ_', normalized);
end