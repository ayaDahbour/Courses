clc;
clear;
close all;

% Create new model
model = 'Question4_Model';
new_system(model);
open_system(model);

% Add blocks
add_block('simulink/Sources/Step', [model '/Step'], 'Position', [30 50 60 80]);
add_block('simulink/Math Operations/Sum', [model '/Sum1'], 'Position', [100 50 130 80], 'Inputs','+-');
add_block('simulink/Continuous/Integrator', [model '/Integrator1'], 'Position', [170 50 200 80]);
add_block('simulink/Math Operations/Gain', [model '/Gain2'], 'Position', [240 50 270 80], 'Gain', '2');
add_block('simulink/Continuous/Integrator', [model '/Integrator2'], 'Position', [170 150 200 180]);
add_block('simulink/Math Operations/Sum', [model '/Sum2'], 'Position', [320 100 350 130], 'Inputs','+-');
add_block('simulink/Continuous/Transfer Fcn', [model '/TransferFcn'], 'Position', [400 100 450 130], 'Numerator','1','Denominator','[1 3]');
add_block('simulink/Sinks/Scope', [model '/Scope'], 'Position', [520 100 550 130]);
add_block('simulink/Math Operations/Gain', [model '/Gain3'], 'Position', [450 200 480 230], 'Gain', '3');

% Connect blocks
add_line(model, 'Step/1', 'Sum1/1');
add_line(model, 'Sum1/1', 'Integrator1/1');
add_line(model, 'Integrator1/1', 'Gain2/1');
add_line(model, 'Integrator1/1', 'Integrator2/1');
add_line(model, 'Gain2/1', 'Sum2/2');
add_line(model, 'Integrator2/1', 'Sum2/1');
add_line(model, 'Sum2/1', 'TransferFcn/1');
add_line(model, 'TransferFcn/1', 'Scope/1');
add_line(model, 'TransferFcn/1', 'Gain3/1');
add_line(model, 'Gain3/1', 'Sum1/2');

% Save and open model
save_system(model);
open_system(model);
