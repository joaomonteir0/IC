#!/bin/bash

# Encode and decode for each predictor
for i in {0..5}; do
    echo "Testing predictor $i..."
    ./video_codec encode teste.y4m encoded_test_$i.bin $i
    ./video_codec decode encoded_test_$i.bin decoded_test_$i.mp4
done

# Compare decoded outputs with original
for i in {0..5}; do
    echo "Comparing original with predictor $i..."
    ./compare teste.y4m decoded_test_$i.mp4
done
