function text = text_fixture(name)
    [supportPath, ~, ~] = fileparts(mfilename('fullpath'));
    [testPath, ~, ~] = fileparts(supportPath);
    fixturePath = fullfile(testPath, 'fixtures');

    text = fileread(fullfile(fixturePath, name));
end