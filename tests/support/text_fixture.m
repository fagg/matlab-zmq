function text = text_fixture(name)
    [supportPath, ~, ~] = fileparts(mfilename('fullpath'));
    [testPath, ~, ~] = fileparts(supportPath);
    fixturePath = fullfile(testPath, 'fixtures');

    fid = fopen(fullfile(fixturePath, name), 'r');
    data = fread(fid)';
    fclose(fid);
    text = char(data);
end