curl https://api.openai.com/v1/images/generations \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "prompt": "Generate an image about: the theme is using openai api in c language tutorial,the image should be well optimized in color,smooth,and charming",
    "n": 1,
    "size": "512x512"
  }'
