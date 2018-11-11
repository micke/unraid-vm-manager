FROM ruby:2.5
MAINTAINER Micke Lisinge <me@mike.gg>

RUN apt-get update && apt-get install -y --no-install-recommends \
    libvirt-dev \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY Gemfile* .
RUN bundle install

# Upload source
COPY . .

# Start server
ENV PORT 3000
EXPOSE 3000
CMD ["ruby", "app.rb"]
