%% run_hack

% Generate New hack_data
% read data from files
file_path = "Data/";
file_mask = "*.gif";
img_path_list = dir(strcat(file_path, file_mask));
img_num = length(img_path_list);
X = [];
Y = [];
digit_martix = [];
%convert filenames as the labels of figures
for i = 1:img_num
    img_name = img_path_list(i).name;
    img_name_array = convertStringsToChars(img_name);
    img_name_array_with_path = convertStringsToChars("Data/" + img_name);
    temp = extract_image(img_name_array_with_path);
    X = [X, temp];
    five_digits = img_name_array(1:5);
    digit_martix = [];
    for j = five_digits
        digit_martix = [digit_martix,  str2double(j)];
    end
    Y = [Y,digit_martix];
end

% save hack_data
save("hack_data.mat",'X','Y');

% hack
file_path = "Test/";
file_mask = "*.gif";
img_path_list = dir(strcat(file_path, file_mask));
img_num = length(img_path_list);
for i = 1:img_num
    img_name = img_path_list(i).name;
    img_name_array_with_path = convertStringsToChars(file_path + img_name);
    hack(img_name_array_with_path);
end
