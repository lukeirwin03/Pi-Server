# Use a more standard Python image
FROM python:3.10-slim

# Install build dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    make \
    bash \
    libffi-dev \
    libssl-dev \
    python3-dev \
    build-essential \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy application code and script into the container
COPY . /app

# Upgrade pip and install Python packages globally
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

# Make the script executable
RUN chmod +x /app/start.sh

# Expose the port that your server will run on
EXPOSE 7703

# Run the start script
CMD ["/app/start.sh"]
