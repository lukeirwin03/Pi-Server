FROM alpine:latest

# Install dependencies
RUN apk update && apk add --no-cache \
    gcc \
    g++ \
    python3 \
    py3-pip \
    make

WORKDIR /app

# Copy code
COPY . /app

# Install Python packages
RUN pip3 install -r requirements.txt

# Expose port
EXPOSE 7703

RUN chmod +x /app/start.sh
RUN /app/start.sh