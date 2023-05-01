curl https://api.openai.com/v1/images/generations \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "prompt": "Please write a lovely dog and her friend leo who is a lion in Paris",
    "n": 1,
    "size": "512x512"
  }'
