#!/usr/bin/env python3
import subprocess, csv, os.path, sys, re, random, glob
from random import randrange

random.seed()
re_time = re.compile('^time (ours|naive): ((?:\\d)+)μs$', flags=re.MULTILINE)
re_max_genes = re.compile('^max_gene: ((?:\\d)+)$', flags=re.MULTILINE)
n_form = 10
mat_dir = 'mats'
results_existed = os.path.exists('results.csv')
with open('results.csv', 'a+', newline='') as ofile:
    writer = csv.DictWriter(ofile, ['mat file', 'num clauses', 'max literals',
                                    'max genes', 'not probability',
                                    'our time', 'naive time'])
    if not results_existed:
        writer.writeheader()
    for i in range(20):
        fields = {'num clauses': 1 + randrange(80),
                  'max literals': 1 + randrange(80),
                  'max genes': random.choice([None, randrange(80, 23000, 1)]),
                  # miniumum number of genes because duplicate genes are pointless
                  'not probability': random.uniform(0.0, 1.0)}
        for mat in map(os.path.basename, glob.iglob(os.path.join(mat_dir, '*.mat'))):
            fields['mat file'] = os.path.splitext(mat)[0]
            form_file = os.path.join('forms',
                                     ''.join([os.path.splitext(mat)[0], '_',
                                              str(i), '.txt']))
            cmd = [os.path.join('.', 'formula'), os.path.join(mat_dir, mat),
                           '-o', form_file, '-n', str(10),
                           '-c', str(fields['num clauses']),
                           '-p', str(fields['not probability']),
                           '-l', str(fields['max literals'])]
            if fields['max genes']:
                cmd.extend(['-g', str(fields['max genes'])])

            res = subprocess.run(cmd, capture_output=True)
            print(res.stdout.decode())
            if res.returncode:
                print(cmd)
                print(f'{res.returncode} when generating formula for {mat}',
                      file=sys.stderr)
                print(res.stderr.decode(), file=sys.stderr)
            else:
                fields['max genes'] = int(re_max_genes.search(res.stdout.decode())[1])
                cmd = [os.path.join('.', 'test'), 'query', form_file,
                       os.path.join(mat_dir, mat)]
                res = subprocess.run(cmd, capture_output=True)
                if res.returncode:
                    print(f'Error {res.returncode} when testing {mat}',
                          file=sys.stderr)
                    print(res.stderr.decode(), file=sys.stderr)
                else:
                    print(res.stdout.decode())
                    matches = re_time.findall(res.stdout.decode())
                    print(matches)
                    if len(matches) == 2:
                        fields['our time'] = matches[0][1]
                        fields['naive time'] = matches[1][1]
                        writer.writerow(fields)
                    else:
                        print(f'Expected 2, got {len(matches)}')
