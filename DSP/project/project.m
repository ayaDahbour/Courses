% Load the file css.mat
load('css.mat');

% Repeat the CSS sequence five times
numBlocks = 5;
cssRepeated = repmat(css, numBlocks, 1);

% Reshape cssRepeated and path to column vectors
cssRepeated = cssRepeated(:);
path = path(:);

% Generate the echo signal
echo = conv(cssRepeated, path); % Convolve repeated CSS with impulse response (echo path)

% Calculate the input and output powers
N = length(cssRepeated); % Length of the CSS sequence
inputPower = 10 * log10(sum(abs(cssRepeated).^2) / N);
outputPower = 10 * log10(sum(abs(echo).^2) / length(echo));

% Calculate the difference in power
powerDifference = inputPower - outputPower;

% Plot the resulting echo signal
figure;
plot(echo);
title('Echo Signal');
xlabel('Sample');
ylabel('Amplitude');

% Display the amplitudes of power and the difference
fprintf('Input Power: %.2f\n', inputPower);
fprintf('Output Power: %.2f\n', outputPower);
fprintf('Power Difference: %.2f\n', powerDifference);